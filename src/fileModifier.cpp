#include "fileModifier.hpp"
#include <qdebug.h>
FileModifier::FileModifier(const Task &task, QObject *parent)
    : QObject(parent), _task(task), _stopRequested(false) {}

void FileModifier::requestStop() { _stopRequested = true; }

void FileModifier::onProcess() {
  bool running = true;
  emit progress({._info = "started"});
  while (running && !_stopRequested) {
    // TODO: implement
    emit progress({._info = "in progress"});
    break;
  }

  emit finished({._info = "completed"});
}
