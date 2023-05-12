#include "login.h"
#include "ui_login.h"
#include "ventanaprincipal.h"

#include <QDebug>
#include <QFile>

Login::Login(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Login),
                                manager( new QNetworkAccessManager( this ) )

{
    ui->setupUi(this);


    connect( ui->pbIngresar, SIGNAL(pressed()), this, SLOT(slot_validar_con_API()));
    connect( manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(slot_descargaFinalizada(QNetworkReply*)));

    ui->leUsuario->setFocus();
}

Login::~Login()
{
    delete ui;
}

void Login::slot_validar_con_API()  {

    // Aqui armar la solicitud para el webservice

    if (intentosFallidos >= 3 && tiempoUltimoIntento.secsTo(QDateTime::currentDateTime()) < 60) {
        int tiempoRestante = 60 - tiempoUltimoIntento.secsTo(QDateTime::currentDateTime());
        QString mensaje = QString("Ha excedido el número máximo de intentos fallidos. Por favor, inténtelo de nuevo en %1 segundo(s).").arg(tiempoRestante);
        QMessageBox::warning(this, "Cuenta bloqueada", mensaje);
    }else{
        if (tiempoUltimoIntento.secsTo(QDateTime::currentDateTime()) >= 60) {
            intentosFallidos = 0; // Restablecer la variable si ha pasado un minuto
        }
        QString sUrl = "https://whatsyourmovie.com/POO/Ejercicio16.php?email=";
        sUrl += ui->leUsuario->text();
        sUrl += "&password=";
        sUrl += ui->leClave->text();

        manager->get( QNetworkRequest( QUrl( sUrl ) ) );
        tiempoUltimoIntento = QDateTime::currentDateTime(); // Establecer el tiempo del último intento
    }
}

/**
 * @brief Login::slot_descargaFinalizada se ejecuta cuando la respuesta llega aqui
 * @param reply es toda la info que mando el server
 */
void Login::slot_descargaFinalizada( QNetworkReply * reply )  {
    VentanaPrincipal* ventana = new VentanaPrincipal();
    QByteArray ba = reply->readAll();
    QString response = QString::fromUtf8(ba);

    if (response.contains("You are In!!\n")) {
        QMessageBox::information(this, "Inicio de sesión exitoso", "Ha iniciado sesión correctamente.");
        loginExitoso = true;

        QFile file( "/Users/nico/Facultad/3_Año/POO/GitHub/UBP_POO_2023/apiGoogle.html" );
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        ventana->setContenido( file.readAll() );
        ventana->show();

    } else {
        QMessageBox::warning(this, "Error de inicio de sesión", "El usuario o la contraseña son incorrectos.");
        intentosFallidos++;
        if (intentosFallidos >= 3) {
            tiempoUltimoIntento = QDateTime::currentDateTime(); // Establecer el tiempo del último intento
        }
    }
}


