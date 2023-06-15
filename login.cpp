/**
 * @file login.cpp
 *
 * @brief Este archivo contiene la implementación del logIn a la app Merpido
 */

#include "login.h"
#include "ui_login.h"
#include "mercadolibre.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login),
    session_url("https://whatsyourmovie.com/POO/Ejercicio16.php?email="),
    weatherManager(new QNetworkAccessManager(this)),
    userManager(new QNetworkAccessManager(this))
{
    // Configurar la interfaz de usuario
    ui->setupUi(this);

    // Validar el clima a través de la API del clima
    wheatherValidateApi();
    connect(weatherManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(wheatherprocessAnswer(QNetworkReply*)));

    // Conectar el botón de inicio de sesión con la validación de inicio de sesión
    connect( ui->pushButton_logIn, SIGNAL(pressed()), this, SLOT(logInValidateApi()));
    connect( ui->lineEdit_userPassword, SIGNAL(returnPressed()), this, SLOT(logInValidateApi()));
    connect( userManager, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(logInProcessAnswer(QNetworkReply*)));

    // Leer las credenciales guardadas desde el archivo de configuración
    QSettings settings("miapp.ini", QSettings::IniFormat);
    QString usuarioGuardado = settings.value("login/usuario").toString();
    QString contrasenaGuardada = settings.value("login/contrasena").toString();

    // Establecer los valores guardados en los campos de texto
    ui->lineEdit_userName->setText(usuarioGuardado);
    ui->lineEdit_userPassword->setText(contrasenaGuardada);
}

Login::~Login()
{
    delete ui;
}

/**
 * @brief Realiza la validación de la API del clima.
 *
 * Esta función realiza una solicitud a la API del clima para obtener los datos de temperatura.
 * Establece la conexión con la señal `finished` del objeto `weatherManager` para procesar la respuesta.
 */
void Login::wheatherValidateApi()
{
    QString url = "https://api.openweathermap.org/data/2.5/weather?lat=-31.420082&lon=-64.188774&appid=3fea7d41cc5455e7df4141c535a32703&units=metric";
    weatherManager->get(QNetworkRequest(QUrl(url)));
}

/**
 * @brief Procesa la respuesta de la API del clima.
 *
 * Esta ranura se activa cuando se recibe una respuesta de la API del clima.
 * Extrae la temperatura de la respuesta JSON y actualiza la etiqueta de temperatura en la interfaz de usuario.
 * También establece el estilo de la etiqueta de imagen según la temperatura.
 *
 * @param reply La respuesta de red que contiene los datos del clima.
 */
void Login::wheatherprocessAnswer(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonValue temperaturaValue = jsonDoc["main"]["temp"];
    double temperatura = temperaturaValue.toDouble();

    QString textoTemperatura = "Temperatura: " + QString::number(temperatura) + " °C";
    ui->labelTemperaturaActual->setText(textoTemperatura);

        if (temperatura > 15) {
            ui->ImagenTiempo->setStyleSheet(R"(
                QLabel#ImagenTiempo {
                    background-image: url(:/imagenes/Soleado.png);
                    background-repeat: no-repeat;
                    background-position: center;
                    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));
                    border-radius: 10px;
                }
            )");
        } else if (temperatura < 15 && temperatura > 10) {
            ui->ImagenTiempo->setStyleSheet(R"(
                QLabel#ImagenTiempo {
                    background-image: url(:/imagenes/Ventoso.png);
                    background-repeat: no-repeat;
                    background-position: center;
                    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));
                    border-radius: 10px;
                }
            )");
        } else if (temperatura < 10) {
            ui->ImagenTiempo->setStyleSheet(R"(
                QLabel#ImagenTiempo {
                    background-image: url(:/imagenes/Helado.png);
                    background-repeat: no-repeat;
                    background-position: center;
                    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 178, 102, 255), stop:0.55 rgba(235, 148, 61, 255), stop:0.98 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 0));
                    border-radius: 10px;
                }
            )");
        }
}

/**
 * @brief Realiza la validación de inicio de sesión a través de la API.
 *
 * Esta función realiza una solicitud a la API de inicio de sesión con las credenciales proporcionadas.
 * Verifica el número de intentos fallidos y el tiempo transcurrido para bloquear la cuenta en caso necesario.
 * Establece la conexión con la señal `finished` del objeto `userManager` para procesar la respuesta.
 */
void Login::logInValidateApi()
{
    if (intentosFallidos >= 3 && tiempoUltimoIntento.secsTo(QDateTime::currentDateTime()) < 60) {
           int tiempoRestante = 60 - tiempoUltimoIntento.secsTo(QDateTime::currentDateTime());
           QString mensaje = QString("Ha excedido el número máximo de intentos fallidos. Por favor, inténtelo de nuevo en %1 segundo(s).").arg(tiempoRestante);
           QMessageBox::warning(this, "Cuenta bloqueada", mensaje);
       }else{
           if (tiempoUltimoIntento.secsTo(QDateTime::currentDateTime()) >= 60) {
               intentosFallidos = 0; // Restablecer la variable si ha pasado un minuto
           }

           session_url += ui->lineEdit_userName->text();
           session_url += "&password=";
           session_url += ui->lineEdit_userPassword->text();

           userManager->get( QNetworkRequest( QUrl( session_url ) ) );
           tiempoUltimoIntento = QDateTime::currentDateTime(); // Establecer el tiempo del último intento
       }
}

/**
 * @brief Procesa la respuesta de la API de inicio de sesión.
 *
 * Esta ranura se activa cuando se recibe una respuesta de la API de inicio de sesión.
 * Verifica si las credenciales son válidas y muestra un mensaje de éxito o error.
 * Si el inicio de sesión es exitoso, guarda las credenciales si la casilla "Recordarme" está marcada.
 * Oculta la ventana de inicio de sesión y muestra la ventana de MercadoLibre.
 *
 * @param second_reply La respuesta de red que contiene la respuesta del inicio de sesión.
 */
void Login::logInProcessAnswer(QNetworkReply * second_reply)
{
    QByteArray ba = second_reply->readAll();
    QString response = QString::fromUtf8(ba);

    if (response.contains("Sorry, credentials dont match")) {
           QMessageBox::warning(this, "Error de inicio de sesión", "El usuario o la contraseña son incorrectos.");
           intentosFallidos++;
           if (intentosFallidos >= 3) {
               tiempoUltimoIntento = QDateTime::currentDateTime(); // Establecer el tiempo del último intento
           }

        } else {

            QMessageBox::information(this, "Inicio de sesión exitoso", "Ha iniciado sesión correctamente.");
            loginExitoso = true;
            if(ui->checkBox_Recordarme->isChecked())
            {
                QSettings settings("miapp.ini", QSettings::IniFormat);
                settings.setValue("login/usuario", ui->lineEdit_userName->text());
                settings.setValue("login/contrasena", ui->lineEdit_userPassword->text());
            }

            hide();
            mercadoLibre* m = new mercadoLibre(nullptr,session_url);
            m->show();
        }
}

/**
 * @brief Procesa la respuesta al hacer click en el boton Cancel
 *
 * Esta ranura se activa cuando se recibe una señal de click en el boton Cancel
 * Realiza una consulta al usuario si quiere salir de la aplicacion
 */
void Login::on_pushButton_cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question Message","Close Merpido App?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}
