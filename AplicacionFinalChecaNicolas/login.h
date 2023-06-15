#ifndef LOGIN_H
#define LOGIN_H

#include "mercadolibre.h"
#include <QWidget>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QUrl>
#include <QString>
#include <QSettings>

/**
 * @brief La clase Login representa un widget de inicio de sesión para la aplicación.
 *
 * La clase Login es un QWidget que permite a los usuarios iniciar sesión en la aplicación.
 * Se comunica con la base de datos (WhatsYourMovie) para validar las credenciales de usuario y con la api de openweathermap para obtener datos climáticos.
 * En el caso que se realiza un login exitoso, se ingresa a la app la cual permite buscar de manera rapida y eficientes los productos mas baratos de mercado libre
 */

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construye un objeto Login.
     *
     * @param parent El QWidget padre (opcional).
     */
    Login(QWidget *parent = nullptr);
    /**
     * @brief Destruye el objeto Login.
     */
    ~Login();

private:
    Ui::Login *ui; ///< El objeto UI para el widget de inicio de sesión.

    QString session_url; ///< La URL para la sesión. (Se utilizara para el LogIn como para acceder a MercadoLibre)

    QNetworkAccessManager *weatherManager; ///< El administrador de acceso a la red para los datos climáticos.
    QNetworkAccessManager * userManager; ///< El administrador de acceso a la red para los datos de usuario.
    int intentosFallidos = 0; ///< El número de intentos de inicio de sesión fallidos.
    QDateTime tiempoUltimoIntento; ///< La marca de tiempo del último intento de inicio de sesión.
    bool loginExitoso = false;  ///< Variable que indica si el inicio de sesión fue exitoso.

private slots:
    /**
     * @brief Ranura para el evento de clic del botón "Cancelar".
     *
     * Esta ranura se activa cuando el usuario hace clic en el botón "Cancelar".
     */
    void on_pushButton_cancel_clicked();

    /**
    * @brief Valida la API del clima.
    *
    * Esta ranura se activa para validar la API del clima.
    */
    void wheatherValidateApi();

    /**
     * @brief Procesa la respuesta de la API del clima.
     *
     * Esta ranura se activa cuando se recibe una respuesta de la API del clima.
     *
     * @param reply La respuesta de red que contiene los datos del clima.
     */
    void wheatherprocessAnswer(QNetworkReply *reply);

    /**
     * @brief Valida la API de inicio de sesión.
     *
     * Esta ranura se activa para validar la API de inicio de sesión.
     */
    void logInValidateApi();

    /**
     * @brief Procesa la respuesta de la API de inicio de sesión.
     *
     * Esta ranura se activa cuando se recibe una respuesta de la API de inicio de sesión.
     *
     * @param second_reply La respuesta de red que contiene la respuesta del inicio de sesión.
     */
    void logInProcessAnswer(QNetworkReply * second_reply);
};
#endif // LOGIN_H
