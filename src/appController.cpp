#include "appController.hpp"
#include "fileScheduler.hpp"
#include "src/fileModifier.hpp"
#include <QRegularExpression>
#include <chrono>
#include <qguiapplication.h>
#include <qlogging.h>
#include <qnamespace.h>
#include <qthread.h>

namespace {
static constexpr int s_defaultChunkSizeBytes = 1024 * 256; // 256 kb
static constexpr int s_defaultMaxConcurrentProcesses = 10;
} // namespace

AppController::AppController(QObject *parent)
    : QObject(parent), _scheduler(FileScheduler::Task{._run = false}),
      _taskModel(new TaskModel(this)) {
  QObject::connect(&_scheduler, &FileScheduler::showUserInfo,
                   [&](const QString &msg) { appendLog(msg); });
  QObject::connect(
      &_scheduler, &FileScheduler::progressTask, this,
      [this](const FileModifier::Progress &progress) {
        _taskModel->addTask(progress._taskName);
        _taskModel->updateProgress(progress._taskName,
                                   progress._completePercent);
      },
      Qt::QueuedConnection);

  QObject::connect(
      &_scheduler, &FileScheduler::finishedTask, this,
      [this](const FileModifier::Progress &progress) {
        // _taskModel.finishTask(progress._taskName);
      },
      Qt::QueuedConnection);
}

void AppController::start() {
  FileScheduler::Task task;
  task._fileRepeatAction = _fileRepeatAction;
  task._fromDirectory = _inputDir;
  task._toDirectory = _outputDir;
  task._byteMask = QByteArray::fromHex(_byteMask.toUtf8());
  task._maxconcurrentProcesses = s_defaultMaxConcurrentProcesses;
  task._chunkSizeBytes = s_defaultChunkSizeBytes;
  task._queryInterval = std::chrono::milliseconds{_queryIntervalMs};
  task._inputFileMask = _fileMask;
  task._run = true;
  task._singleShot = _singleShot;
  task._deleteOnModify = _deleteOnModify;
  _scheduler.setTask(task);
}

void AppController::stop() {
  FileScheduler::Task task{_scheduler.getTask()};
  task._run = false;
  _scheduler.setTask(task);
}

const QString &AppController::getStatus() const { return _status; }

const QString &AppController::getInputDir() const { return _inputDir; }

const QString &AppController::getOutputDir() const { return _outputDir; }

const QString &AppController::getFileMask() const { return _fileMask; }

const QString &AppController::getByteMask() const { return _byteMask; }

int AppController::getQueryIntervalMs() const { return _queryIntervalMs; }

bool AppController::getSingleShot() const { return _singleShot; }

const QString &AppController::getLog() const { return _log; }

void AppController::setStatus(const QString &param) { _status = param; }

void AppController::setInputDir(const QString &param) {
  if (_inputDir == param)
    return;
  _inputDir = param;
  emit inputDirChanged();
}

void AppController::setOutputDir(const QString &param) {
  if (_outputDir == param)
    return;
  _outputDir = param;
  emit outputDirChanged();
}

void AppController::setFileMask(const QString &param) {
  if (_fileMask == param)
    return;
  _fileMask = param;
  emit fileMaskChanged();
}

void AppController::setByteMask(const QString &param) {

  if (_byteMask == param)
    return;

  _byteMask = param;
  emit byteMaskChanged();
}

void AppController::setQueryIntervalMs(int param) {
  if (_queryIntervalMs == param)
    return;
  _queryIntervalMs = param;
  emit queryIntervalMsChanged();
}

void AppController::setSingleShot(bool param) {
  if (_singleShot == param)
    return;
  _singleShot = param;
  emit singleShotChanged();
}

void AppController::appendLog(const QString &msg) {
  static constexpr int MaxLines = 100;

  QStringList lines = _log.split('\n', Qt::SkipEmptyParts);
  lines.append(msg);

  while (lines.size() > MaxLines) {
    lines.removeFirst();
  }

  _log = lines.join('\n') + '\n';
  emit logChanged();
}

bool AppController::getDeleteOnModify() const { return _deleteOnModify; }

void AppController::setDeleteOnModify(bool param) {
  _deleteOnModify = param;
  emit deleteOnModifyChanged();
}

void AppController::setRepeatAction(FileScheduler::FileRepeatAction param) {
  _fileRepeatAction = param;
  emit repeatActionChanged();
}

FileScheduler::FileRepeatAction AppController::getRepeatAction() {
  return _fileRepeatAction;
}

TaskModel *AppController::getTasks() { return _taskModel; }
