#include "fileModifier.hpp"
FileModifier::FileModifier(const Task &task, QObject *parent)
    : QObject(parent), _task(task), _stopRequested(false) {}

void FileModifier::requestStop() { _stopRequested = true; }

void FileModifier::onProcess() {
  bool running = true;
  while (running && !_stopRequested) {
    // TODO: implement
    emit progress({});
    break;
  }

  emit finished({});
}
