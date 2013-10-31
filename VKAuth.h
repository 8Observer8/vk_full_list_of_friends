#ifndef VKAUTH_H
#define VKAUTH_H

#include <QWebView>
#include <QNetworkReply>
#include <QtXml>

class VKAuth : public QWebView
{
    Q_OBJECT

protected:
    QString m_app;
    QNetworkReply* m_http;
    void loadLoginPage();

public:
    VKAuth(QString app, QWidget* parent=0);

signals:
    void success(QDomDocument);
    void unsuccess();

public slots:
    void slotLinkChanged(QUrl url);
    void slotDone();
};

#endif // VKAUTH_H
