#include "VKAuth.h"
#include <QNetworkRequest>
#include <QMessageBox>

void VKAuth::loadLoginPage(){
    QUrl url("https://oauth.vk.com/authorize");

    url.addQueryItem("client_id", m_app);
    url.addQueryItem("layout","https://oauth.vk.com/blank.html"  );
    url.addQueryItem("display", "popup");
    url.addQueryItem("scope", "8");
    url.addQueryItem("response_type", "token");

    load(url);
}

VKAuth::VKAuth(QString app,QWidget* parent) : QWebView(parent)
{

    QObject::connect(this, SIGNAL( urlChanged(QUrl)      ),
                     SLOT( slotLinkChanged(QUrl) )
                     );
    m_app = app;
    loadLoginPage();
}

void VKAuth::slotLinkChanged(QUrl url)
{
    if ("/blank.html" == url.path()) {
        QRegExp regexp("access_token=([^,]+)&expires_in=([^,]+)&user_id=([^,]+)");

        QString str= url.fragment();

        if( -1 != regexp.indexIn(str) ){
            QString access_token = regexp.cap(1);
            QString expires_in = regexp.cap(2);
            QString user_id = regexp.cap(3);

            QUrl request(QString("https://api.vk.com/method/%1.xml?").arg("friends.get"));
            request.addQueryItem("user_id", user_id);
            request.addQueryItem("fields", "online");
            request.addQueryItem("v", "5.2");
            request.addQueryItem("access_token", access_token);

            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            m_http = manager->get(QNetworkRequest(request));
            QObject::connect(m_http, SIGNAL(finished()), this, SLOT(slotDone()));
        }
    }
    else if("/api/login_failure.html" == url.path()){
        emit unsuccess();
    }
}

void VKAuth::slotDone() {
    QString russian = QString::fromUtf8(m_http->readAll());
    qDebug() << russian;
    QDomDocument dom;
    QByteArray aByteArray = russian.toUtf8();
    if (!dom.setContent(aByteArray)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to parse the file into a DOM tree"));
        return;
    }

    emit success(dom);
    hide(); //hide this window
    m_http->close();
}
