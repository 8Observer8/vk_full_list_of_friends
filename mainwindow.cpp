#include "mainwindow.h"
#include <QtGui>
#include <QtXml>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    table = new QTableView;

    // Online Radio
    QRadioButton *allRadio = new QRadioButton(tr("All"));
    allRadio->setChecked(true);
    QRadioButton *onlineRadio = new QRadioButton(tr("Online"));
    QRadioButton *offlineRadio = new QRadioButton(tr("Offline"));

    // Online Group
    QButtonGroup *onlineGroup = new QButtonGroup;
    onlineGroup->addButton(allRadio);
    onlineGroup->addButton(onlineRadio);
    onlineGroup->addButton(offlineRadio);

    // Online Layout
    QVBoxLayout *onlineLayout = new QVBoxLayout;
    onlineLayout->addWidget(allRadio);
    onlineLayout->addWidget(onlineRadio);
    onlineLayout->addWidget(offlineRadio);

    // Online Box
    QGroupBox *onlineBox = new QGroupBox;
    onlineBox->setTitle(tr("Show"));
    onlineBox->setLayout(onlineLayout);

    // Control Layout
    QVBoxLayout *controlLayout = new QVBoxLayout;
    controlLayout->addWidget(onlineBox);
    controlLayout->addStretch();

    // Main Layout
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(table);
    mainLayout->addLayout(controlLayout);

    QWidget *window = new QWidget;
    window->setLayout(mainLayout);

    setCentralWidget(window);

    this->setWindowTitle("Friend Table");

    connect(allRadio, SIGNAL(clicked()), this, SLOT(allRadio_clicked_slot()));
    connect(onlineRadio, SIGNAL(clicked()), this, SLOT(onlineRadio_clicked_slot()));
    connect(offlineRadio, SIGNAL(clicked()), this, SLOT(offlineRadio_clicked_slot()));
}

void MainWindow::allRadio_clicked_slot() {
    setHeader(model);
    table->setModel(model);
}

void MainWindow::onlineRadio_clicked_slot() {
    int count = countOnline();
    int nrows = count;
    modelOnline = new QStandardItemModel(nrows, ncols, this);
    int total = model->rowCount();

    QModelIndex index;
    QModelIndex indexOnline;
    int rowOnline = 0;
    int colOnline = 0;
    for (int row = 0; row < total; row++) {
        if (model->index(row, 3).data().toInt() == 1) {
            for (int col = 0; col < ncols; col++) {
                indexOnline = modelOnline->index(rowOnline, colOnline);
                colOnline++;
                index = model->index(row, col);
                modelOnline->setData(indexOnline, model->data(index).toString());
            }
            rowOnline++;
            colOnline = 0;
        }
    }

    setHeader(modelOnline);
    table->setModel(modelOnline);
}

void MainWindow::setHeader(QStandardItemModel* m) {
    m->setHeaderData(0, Qt::Horizontal, tr("id"));
    m->setHeaderData(1, Qt::Horizontal, tr("First Name"));
    m->setHeaderData(2, Qt::Horizontal, tr("Last Name"));
    m->setHeaderData(3, Qt::Horizontal, tr("Online/Offline"));
}

int MainWindow::countOnline() {
    int countOnline = 0;

    int total = model->rowCount();
    for (int row = 0; row < total; row++) {
        if (model->index(row, 3).data().toInt() == 1) {
            countOnline++;
        }
    }

    return countOnline;
}

int MainWindow::countOffline() {
    int countOffline = 0;

    int total = model->rowCount();
    for (int row = 0; row < total; row++) {
        if (model->index(row, 3).data().toInt() == 0) {
            countOffline++;
        }
    }

    return countOffline;
}

void MainWindow::offlineRadio_clicked_slot() {
    int count = countOffline();
    int nrows = count;
    modelOffline = new QStandardItemModel(nrows, ncols, this);
    int total = model->rowCount();

    QModelIndex index;
    QModelIndex indexOffline;
    int rowOffline = 0;
    int colOffline = 0;
    for (int row = 0; row < total; row++) {
        if (model->index(row, 3).data().toInt() == 0) {
            for (int col = 0; col < ncols; col++) {
                indexOffline = modelOffline->index(rowOffline, colOffline);
                index = model->index(row, col);
                modelOffline->setData(indexOffline, model->data(index).toString());
                colOffline++;
            }
            rowOffline++;
            colOffline = 0;
        }
    }

    setHeader(modelOffline);
    table->setModel(modelOffline);
}

void MainWindow::slotSuccess(QDomDocument dom) {
    QDomElement root = dom.firstChildElement();
    QDomElement countElement = root.firstChildElement();
    QDomElement itemsElement = countElement.nextSiblingElement();
    QDomElement userElement = itemsElement.firstChildElement();
    QDomElement idElement;
    QDomElement firstNameElement;
    QDomElement lastNameElement;
    QDomElement onlineElement;

    int count = countElement.text().toInt();
    int nrows = count;
    ncols = 4;
    model = new QStandardItemModel(nrows, ncols, this);

    QModelIndex index;
    for (int row = 0; row < nrows; row++) {
        idElement = userElement.firstChildElement();
        index = model->index(row, 0);
        model->setData(index, idElement.text());

        firstNameElement = idElement.nextSiblingElement();
        index = model->index(row, 1);
        model->setData(index, firstNameElement.text());

        lastNameElement = firstNameElement.nextSiblingElement();
        index = model->index(row, 2);
        model->setData(index, lastNameElement.text());

        onlineElement = lastNameElement.nextSiblingElement();
        index = model->index(row, 3);
        model->setData(index, onlineElement.text());

        userElement = userElement.nextSiblingElement();
    }

    setHeader(model);
    table->setModel(model);

    show();
}
