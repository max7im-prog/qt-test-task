#include "appController.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  AppController *controller = new AppController(&app);

  qmlRegisterSingletonInstance("custom.AppController", 1, 0, "AppController",
                               controller);

  QQmlApplicationEngine engine;
  engine.loadFromModule("App", "Main");

  return app.exec();
}
