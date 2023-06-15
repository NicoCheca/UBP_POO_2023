#ifndef MERCADOLIBRE_H
#define MERCADOLIBRE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QStringList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDesktopServices>
#include <QDebug>

namespace Ui {
class mercadoLibre;
}

/**
 * @brief La clase mercadoLibre representa un widget para interactuar con la API de MercadoLibre.
 *
 * La clase mercadoLibre es un QWidget que permite buscar productos en la API de MercadoLibre y obtener información relacionada.
 */
class mercadoLibre : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Construye un objeto mercadoLibre.
     *
     * @param parent El QWidget padre.
     * @param session_url La URL de la sesión.(esto la tomamos desde logIn.h y logIn.cpp)
     */
    mercadoLibre(QWidget *parent, QString session_url);

    /**
     * @brief Destruye el objeto mercadoLibre.
     */
    ~mercadoLibre();

private:
    Ui::mercadoLibre *ui; ///< El objeto UI para el widget mercadoLibre.
    QNetworkAccessManager* meliNetworkBusqueda;  ///< El administrador de acceso a la red para la búsqueda.
    QNetworkAccessManager* meliNetworkObtenerUltimaBusqueda; ///< El administrador de acceso a la red para obtener la última búsqueda.
    QNetworkAccessManager* meliNetworkGuardarUltimabusqueda; ///< El administrador de acceso a la red para guardar la última búsqueda.
    QNetworkAccessManager* meliNetworkImagen; ///< El administrador de acceso a la red para obtener imágenes.

    QString ultimaBusquedaUrl;  ///< La URL de la última búsqueda.
    QStringList imagenesProductoUrl; ///< La lista de URL de imágenes de productos.

    int iterador; ///< El iterador utilizado para procesar las respuestas de la búsqueda.
    int iteradorConsulta = 0;  ///< El iterador utilizado para consultar la última búsqueda.

    void guardarUltimaBusqueda(QString busqueda_url, QString producto); ///< Funcion para guardar la ultima busqueda realizada en la base de datos
    void datosProductos(QJsonArray results_array); ///< Funcion para obtener los datos de los productos de la Api de MercadoLibre
    void imagenProductos(QJsonArray results_array); ///< Funcion para obtener las imagenes de los productos de la Api de MercadoLibre
    void obtenerImagenesUrlProducto(QJsonArray results_array); ///< Funcion para obtener las url de las imagenes de los productos de la Api de MercadoLibre
    void obtenerImagenes(QStringList imagenes_url);  ///< Funcion las imagenes de la Api de MercadoLibre

private slots:
    /**
     * @brief Ranura para el evento de búsqueda.
     *
     * Esta ranura se activa cuando se realiza una búsqueda.
     */
    void busqueda();

    /**
     * @brief Obtiene la última búsqueda realizada.
     *
     * Esta ranura se activa cuando se recibe una respuesta para obtener la última búsqueda.
     *
     * @param m_oub_reply La respuesta de red que contiene la última búsqueda.
     */
    void obtenerUltimaBusqueda(QNetworkReply* m_oub_reply);

    /**
     * @brief Descarga la búsqueda realizada.
     *
     * Esta ranura se activa cuando se recibe una respuesta para descargar la búsqueda.
     *
     * @param m_rb_reply La respuesta de red que contiene los datos de la búsqueda.
     */
    void descargaBusqueda(QNetworkReply* m_rb_reply);

    /**
     * @brief Obtiene las imágenes después de realizar la descarga.
     *
     * Esta ranura se activa cuando se recibe una respuesta para obtener las imágenes.
     *
     * @param m_oi_reply La respuesta de red que contiene las imágenes.
     */
    void obtenerImagenesDescargaRealizada(QNetworkReply* m_oi_reply);

};

#endif // MERCADOLIBRE_H
