#include "src/appController.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qdebug.h>
#include <qqmlapplicationengine.h>

int main(int argc, char *argv[]) {

  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  AppController controller;
  engine.rootContext()->setContextProperty("app", &controller);

  engine.loadFromModule("App", "Main");

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
