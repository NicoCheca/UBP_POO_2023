/**
 * @file mercadolibre.cpp
 *
 * @brief Este archivo contiene la implementación de la clase mercadoLibre,
 *        que es una ventana de búsqueda de productos en MercadoLibre.
 */

#include "mercadolibre.h"
#include "ui_mercadolibre.h"


/**
 * @brief Constructor de la clase mercadoLibre.
 * @param parent Puntero al widget padre.
 * @param session_url URL de la sesión actual.
 */
mercadoLibre::mercadoLibre(QWidget *parent, QString session_url) :
    QWidget(parent),
    ui(new Ui::mercadoLibre),
    meliNetworkBusqueda(new QNetworkAccessManager),
     meliNetworkObtenerUltimaBusqueda(new QNetworkAccessManager),
     meliNetworkGuardarUltimabusqueda(new QNetworkAccessManager),
     meliNetworkImagen(new QNetworkAccessManager),
     ultimaBusquedaUrl(session_url),
     iterador(0),
     imagenesProductoUrl({"1","2","3","4","5"})
{
    // Configurar la interfaz de usuario
    ui->setupUi(this);

    // Conectar las señales a los slots correspondientes
    meliNetworkObtenerUltimaBusqueda->get(QNetworkRequest(QUrl(ultimaBusquedaUrl)));
    connect(meliNetworkObtenerUltimaBusqueda, SIGNAL(finished(QNetworkReply*)), this,SLOT(obtenerUltimaBusqueda(QNetworkReply*)));
    connect(meliNetworkObtenerUltimaBusqueda,SIGNAL(finished(QNetworkReply*)),this,SLOT(busqueda()));
    connect(ui->pushButton_Search,SIGNAL(pressed()),this,SLOT(busqueda()));
    connect(ui->lineEdit_BusquedaMeli,SIGNAL(returnPressed()),this,SLOT(busqueda()));
    connect(meliNetworkBusqueda,SIGNAL(finished(QNetworkReply*)),this,SLOT(descargaBusqueda(QNetworkReply*)));
    connect(meliNetworkImagen,SIGNAL(finished(QNetworkReply*)),this,SLOT(obtenerImagenesDescargaRealizada(QNetworkReply*)));
}

/**
 * @brief Destructor de la clase mercadoLibre.
 */
mercadoLibre::~mercadoLibre()
{
    delete ui;
}

/**
 * @brief Guarda la última búsqueda realizada.
 * @param busqueda_url URL de la búsqueda.
 * @param producto Producto buscado.
 */
void mercadoLibre::guardarUltimaBusqueda(QString busqueda_url, QString producto)
{
    if(iteradorConsulta != 0){

        busqueda_url += "&l_search=";
        busqueda_url += producto;

        qDebug() << "guardarUltima " + busqueda_url;

        meliNetworkGuardarUltimabusqueda->get(QNetworkRequest(QUrl(busqueda_url)));

    }else{
        iteradorConsulta++;
    }
}

/**
 * @brief Procesa los datos de los productos recibidos.
 * @param results_array Array JSON de los productos.
 */
void mercadoLibre::datosProductos(QJsonArray results_array)
{
    if(!results_array.isEmpty())
        {
            QLabel* productos[] = {ui->datosProducto1, ui->datosProducto2, ui->datosProducto3, ui->datosProducto4, ui->datosProducto5};

            for (int i = 0; i<=4; i++)
            {
                QJsonObject results_object = results_array[i].toObject();
                QJsonObject results_object_seller = results_object["seller"].toObject();
                QJsonObject results_object_seller_address = results_object["seller_address"].toObject();
                QJsonObject results_object_seller_address_state = results_object_seller_address["state"].toObject();

                // Obtener los datos del producto
                QString results_title = results_object["title"].toString();
                QString results_seller_nickname = results_object_seller["nickname"].toString();
                QString results_seller_address_state_name = results_object_seller_address_state["name"].toString();
                double results_price = results_object["price"].toDouble();

                QString results_permalink;
                if (results_object.contains("permalink")) {
                    results_permalink = results_object["permalink"].toString();
                } else {
                    // En caso de que la respuesta JSON no contenga el enlace
                    results_permalink = "Enlace no disponible";
                }

                QString datos_producto = "Producto: " + results_title + "\n" +
                                         "Precio: $" +  QString::number(results_price) + "\n"
                                         "Vendedor: " + results_seller_nickname + "\n"
                                         "Ubicacion: " + results_seller_address_state_name + "\n"
                                         "Enlace: " + results_permalink + "\n";

                 // Mostrar los datos del producto en la interfaz de usuario
                productos[i]->setText(datos_producto);
                productos[i]->adjustSize();
            }
        }
}

/**
 * @brief Descarga las imágenes de los productos.
 * @param results_array Array JSON de los productos.
 */
void mercadoLibre::imagenProductos(QJsonArray results_array)
{
    if(!results_array.isEmpty())
        {
            for(int i=0; i<=4; i++)
            {
                QJsonObject results_object = results_array[i].toObject();
                QString results_thumbnail = results_object["thumbnail"].toString();
                imagenesProductoUrl.replace(i,results_thumbnail);
            }

            meliNetworkImagen->get(QNetworkRequest(QUrl(imagenesProductoUrl.at(iterador))));
        }
}

/**
 * @brief Realiza la búsqueda de productos en MercadoLibre.
 */
void mercadoLibre::busqueda()
{
    QString producto = ui->lineEdit_BusquedaMeli->text();

        if(!producto.isEmpty())
        {
            QString meli_url = "https://api.mercadolibre.com/sites/MLA/search?&q=";
            meli_url += producto;
            meli_url += "&sort=price_asc";

            meliNetworkBusqueda->get(QNetworkRequest(QUrl(meli_url)));

            guardarUltimaBusqueda(ultimaBusquedaUrl,producto);
        }
}

/**
 * @brief Slot que se activa cuando se obtiene la última búsqueda realizada.
 * @param m_oub_reply Respuesta de la solicitud.
 */
void mercadoLibre::obtenerUltimaBusqueda(QNetworkReply* m_oub_reply)
{
    QString meli_ultima_busqueda_reply = (QString)m_oub_reply->readAll();

        if(!meli_ultima_busqueda_reply.isEmpty())
        {
            ui->lineEdit_BusquedaMeli->setText(meli_ultima_busqueda_reply);
        }

        qDebug() << "obtenerUltima " + meli_ultima_busqueda_reply;
}

/**
 * @brief Slot que se activa cuando se completa la descarga de la búsqueda de productos.
 * @param m_rb_reply Respuesta de la solicitud.
 */
void mercadoLibre::descargaBusqueda(QNetworkReply* m_rb_reply)
{
    QString meli_reply = (QString)m_rb_reply->readAll();

    QJsonDocument json_doc = QJsonDocument::fromJson(meli_reply.toUtf8());
    QJsonObject main_obj = json_doc.object();
    QJsonArray results_array = main_obj["results"].toArray();

    // Procesar los datos de los productos
    datosProductos(results_array);
    imagenProductos(results_array);
}

/**
 * @brief Slot que se activa cuando se completa la descarga de una imagen de producto.
 * @param m_oi_reply Respuesta de la solicitud.
 */
void mercadoLibre::obtenerImagenesDescargaRealizada(QNetworkReply *m_oi_reply)
{
    QLabel* imagenes_producto[] = {ui->imagenProducto1, ui->imagenProducto2, ui->imagenProducto3, ui->imagenProducto4, ui->imagenProducto5};

    QByteArray imagen_producto_descargada = m_oi_reply->readAll();
    QPixmap imagen_producto;
    imagen_producto.loadFromData(imagen_producto_descargada);

    // Mostrar la imagen del producto en la interfaz de usuario
    imagenes_producto[iterador]->setPixmap(imagen_producto.scaled(200,200));

    iterador++;

    if(iterador <= 4)
    {
        meliNetworkImagen->get(QNetworkRequest(QUrl(imagenesProductoUrl.at(iterador))));
    }
    else
    {
        iterador = 0;
    }
}



