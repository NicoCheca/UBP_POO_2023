#include "login.h"
#include "ui_login.h"

#include <QDebug>

Login::Login(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Login),
                                manager( new QNetworkAccessManager( this ) )
{
    ui->setupUi(this);


    connect( ui->pbIngresar, SIGNAL(pressed()), this, SLOT(slot_validar_con_API()));
    connect( manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(slot_descargaFinalizada(QNetworkReply*)));
}

Login::~Login()
{
    delete ui;
}

void Login::slot_validar_con_API()  {
    // Aqui armar la solicitud para el webservice

    QString sUrl = "https://whatsyourmovie.com/POO/Ejercicio16.php?email=";
    sUrl += ui->leUsuario->text();
    sUrl += "&password=";
    sUrl += ui->leClave->text();

    manager->get( QNetworkRequest( QUrl( sUrl ) ) );
}

/**
 * @brief Login::slot_descargaFinalizada se ejecuta cuando la respuesta llega aqui
 * @param reply es toda la info que mando el server
 */
void Login::slot_descargaFinalizada( QNetworkReply * reply )  {
    QByteArray ba = reply->readAll();
    qDebug() << ba;
}


