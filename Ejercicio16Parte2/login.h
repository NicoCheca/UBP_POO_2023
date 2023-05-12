
#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QNetworkAccessManager>  // Para ejecutar solicitudes GET, POST, etc
#include <QNetworkRequest>  // Contener la info que ira hacia el server
#include <QNetworkReply>  // Contiene la info que viene desde el server
#include <QUrl>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget

{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login * ui;
    QNetworkAccessManager * manager;
    int intentosFallidos = 0;
    QDateTime tiempoUltimoIntento;
    bool loginExitoso = false;

private slots:
    void slot_validar_con_API();
    void slot_descargaFinalizada( QNetworkReply * reply );
};

#endif // LOGIN_H
