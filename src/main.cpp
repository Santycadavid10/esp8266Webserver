#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char* ssid = "CLARO-77C0"; // Nombre de tu red Wi-Fi
const char* password = "Cl4r0@6777C0"; // Contraseña de tu red Wi-Fi

ESP8266WebServer server(80);

void handleRoot() {

     if (LittleFS.exists("pagina.html")) {
        File file = LittleFS.open("pagina.html", "r");
        if (file) {
            // Envía el contenido del archivo al cliente
            server.streamFile(file, "text/html");
            file.close();
        } else {
            Serial.println("Error al abrir el archivo 'pagina.html'");
            server.send(500, "text/plain", "Error interno del servidor");
        }
    } else {
        Serial.println("El archivo 'pagina.html' no existe");
        server.send(404, "text/plain", "Archivo no encontrado");
    }

   
}

void handleRegistrar() {
  // Obtener el documento enviado desde el formulario
  String documento = server.arg("documento");

  // Aquí puedes hacer lo que quieras con el documento recibido, como guardarlo en LittleFS o hacer algo más con él
  Serial.print("Documento recibido: ");
  Serial.println(documento);
  // Enviar una respuesta al cliente
  server.send(200, "text/plain", "Documento recibido correctamente");
}
void setup() {
  Serial.begin(9600);

  // Conéctate a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red Wi-Fi...");
  }
    Serial.print("Conectado a la red Wi-Fi. Dirección IP: ");
  Serial.println(WiFi.localIP());
  // Inicia LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Error al montar LittleFS");
    return;
  }

  Serial.println("LittleFS montado correctamente");

 // Configura la ruta para manejar la solicitud a la raíz "/"
  server.on("/", HTTP_GET, handleRoot);

  // Configura la ruta para manejar las solicitudes POST a "/registrar"
  server.on("/registrar", HTTP_POST, handleRegistrar);

  // Inicia el servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  // Maneja las solicitudes de los clientes
  server.handleClient();
}