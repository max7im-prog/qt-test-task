#include "fileScheduler.hpp"
#include "fileModifier.hpp"
#include <QDir>
#include <QDirIterator>
#include <QThread>
#include <chrono>
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qmutex.h>

FileScheduler::FileScheduler(const Task &task, QObject *parent)
    : QObject(parent), _task(task), _queryTimer(new QTimer(this)),
      _numActiveTasks(0) {
  applyTask();
  QObject::connect(_queryTimer, &QTimer::timeout, this,
                   &FileScheduler::onTimer);
}

void FileScheduler::setTask(const Task &task) {
  {
    QMutexLocker lock(&_taskAccessMutex);
    _task = task;
  }
  applyTask();
  qInfo().noquote() << "set task" << "\n";
}

FileScheduler::Task FileScheduler::getTask() const { return _task; }

void FileScheduler::processQuery() {
  QMutexLocker lock(&_taskAccessMutex);
  QMutexLocker lockPending{&_pendingModifierTasksAccessMutex};

  if (!_pendingModifierTasks.isEmpty()) {
    emit showUserInfo("Still have pending tasks");
    return;
  }

  QDir fromDir(_task._fromDirectory);
  if (!fromDir.exists()) {
    emit showUserInfo("No directory named " + fromDir.dirName() + " exists");
    return;
  }
  QDir toDir(_task._toDirectory);
  if (!toDir.exists()) {
    emit showUserInfo("No directory named " + toDir.dirName() + " exists");
    return;
  }
  if (_task._byteMask.size() != 8) {
    emit showUserInfo("byte mask of size " +
                      QString::number(_task._byteMask.size()) +
                      " bytes, 8 requred");
    return;
  }

  // Apply file mask
  QStringList masks = _task._inputFileMask.split(';', Qt::SkipEmptyParts);
  QDirIterator it(fromDir.absolutePath(), masks,
                  QDir::Files | QDir::NoDotAndDotDot,
                  QDirIterator::NoIteratorFlags);
  while (it.hasNext()) {
    it.next();
    QString inputFilePath = it.filePath();

    auto inputFileInfo = it.fileInfo();
    QString fileName = inputFileInfo.fileName();
    QString outputFilePath = toDir.filePath(inputFileInfo.fileName());

    // Solve file conflicts
    if (QFile::exists(outputFilePath)) {
      switch (_task._fileRepeatAction) {
      case FileScheduler::FileRepeatAction::Copy: { // Add a counter to
                                                    // the file name
        int counter = 1;
        QString basename = inputFileInfo.completeBaseName();
        QString ext = inputFileInfo.suffix();
        QString newName;
        do {
          newName = QString("%1_%2.%3").arg(basename).arg(counter++).arg(ext);
          outputFilePath = toDir.filePath(newName);
        } while (QFile::exists(outputFilePath));
        break;
      }
      case FileScheduler::FileRepeatAction::Pass: { // Move to the next
                                                    // iteration
        continue;
        break;
      }
      case FileScheduler::FileRepeatAction::Overwrite: { // Just overwrite
        break;
      }
      }
    }

    // Prepare FileModifier task
    FileModifier::Task modifierTask;
    modifierTask._byteMask = _task._byteMask;
    modifierTask._fromPath = inputFilePath;
    modifierTask._toPath = outputFilePath;
    modifierTask._chunkSizeBytes = _task._chunkSizeBytes;
    modifierTask._deleteOnModify = _task._deleteOnModify;

    {
      _pendingModifierTasks.push_back(modifierTask);
    }
  }
  {
    while ((!_pendingModifierTasks.isEmpty()) &&
           (_numActiveTasks <= _task._maxconcurrentProcesses)) {
      auto nextTask = _pendingModifierTasks.front();
      _pendingModifierTasks.pop_front();
      scheduleModifier(nextTask);
    }
  }
}

void FileScheduler::applyTask() {
  QMutexLocker lock(&_taskAccessMutex);
  _queryTimer->stop();
  _queryTimer->setSingleShot(_task._singleShot);

  if (_task._queryInterval.count() > 0) {
    _queryTimer->setInterval(_task._queryInterval);
  } else {
    _queryTimer->setInterval(std::chrono::milliseconds{0});
    _queryTimer->setSingleShot(true);
    // Fallback to single shot if the duration is negative
    // TODO: maybe change fallback logic
  }
  if (_task._singleShot) {
    _queryTimer->setInterval(std::chrono::milliseconds{0});
  }

  if (_task._run) {
    _queryTimer->start();
    qInfo().noquote() << "started timer\n";
  }
}

void FileScheduler::onTimer() {

  qInfo().noquote() << "timer fired, querying\n";

  processQuery();
}

void FileScheduler::onModifierProgress(const FileModifier::Progress &progress) {
  emit showUserInfo(progress._info);
}

void FileScheduler::onModifierFinished(const FileModifier::Progress &progress) {
  --_numActiveTasks;
  emit showUserInfo(progress._info);

  // Schedule next task if needed
  {
    QMutexLocker pendingLocker{&_pendingModifierTasksAccessMutex};
    QMutexLocker taskLocker{&_taskAccessMutex};
    if (!_pendingModifierTasks.isEmpty() &&
        _numActiveTasks <= _task._maxconcurrentProcesses) {
      auto nextTask = _pendingModifierTasks.front();
      _pendingModifierTasks.pop_front();
      scheduleModifier(nextTask);
    }
  }
}

void FileScheduler::scheduleModifier(const FileModifier::Task &task) {
  QThread *workerThread = new QThread(this);
  FileModifier *worker = new FileModifier(task);
  worker->moveToThread(workerThread);

  // Lifetime of a worker
  QObject::connect(workerThread, &QThread::started, worker,
                   &FileModifier::onProcess);
  QObject::connect(worker, &FileModifier::finished, workerThread,
                   &QThread::quit);
  QObject::connect(worker, &FileModifier::finished, worker,
                   &QObject::deleteLater);
  QObject::connect(workerThread, &QThread::finished, workerThread,
                   &QObject::deleteLater);

  // Translate info from workers
  QObject::connect(worker, &FileModifier::finished, this,
                   &FileScheduler::onModifierFinished);
  QObject::connect(worker, &FileModifier::progress, this,
                   &FileScheduler::onModifierProgress);
  ++_numActiveTasks;

  workerThread->start();
}
