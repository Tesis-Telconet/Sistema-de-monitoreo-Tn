# Sistema-de-Monitoreo

Este repositorio contiene material diverso para poner en marcha un sistema de monitoreo de amenazas físicas (temperatura, humedad, voltajes, corrientes, calidad de gas). Los datos son recopilados por los distintos tipos de sensores instalados, el proceso de los datos y el envio de información es realizado por las placas Arduino UNO, la información es almacenada en una base de datos SQL. Los datos pueden ser visualizados en un sitio web, este sitio fue desarrollado usando la plantilla ADMIN LTE, para la comunicación entre los Arduinos y la base de datos y / o la base de datos y el sitio web, se desarrollo una APREST escrita en Python que utiliza el framework FLASK.  

El contenido se organiza del siguiente modo:

CASES: diseño delos cases para los Arduinos

Diagramas Proteus: Diagramas de conexión entre sensores y placas Arduino UNO

apirest2: Códigos para la creación de la APIREST y el desarrollo del sitio WEB

codigos finales: Códigos para las placas Arduinos UNO

Planos: Imágenes de las áreas donde se instaló el sistema y el plano del sistema en las instalaciones de Telconet

