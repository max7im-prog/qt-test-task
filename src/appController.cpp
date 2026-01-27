#include "appController.hpp"
#include "fileScheduler.hpp"
#include <QRegularExpression>
#include <chrono>

namespace {
static constexpr int s_defaultChunkSizeBytes = 1024 * 256; // 256 kb
static constexpr int s_defaultMaxConcurrentProcesses = 10;
} // namespace

AppController::AppController(QObject *parent)
    : QObject(parent), _scheduler(FileScheduler::Task{._run = false}) {}

void AppController::start() {
  qInfo().noquote() << "starting" << "\n";
  FileScheduler::Task task;
  task._fileRepeatAction =
      FileScheduler::Task::FileRepeatAction::Copy; // TODO: change to one that
                                                   // was read from GUI
  task._fromDirectory = _inputDir;
  task._toDirectory = _outputDir;
  task._byteMask = _byteMask;
  task._maxconcurrentProcesses = s_defaultMaxConcurrentProcesses;
  task._chunkSizeBytes = s_defaultChunkSizeBytes;
  task._queryInterval = std::chrono::milliseconds{_queryIntervalMs};
  task._inputFileMask = _fileMask;
  task._run = true;
  task._singleShot = _singleShot;
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

QString AppController::getByteMask() const { return _byteMask.toHex(); }

int AppController::getQueryIntervalMs() const { return _queryIntervalMs; }

bool AppController::getSingleShot() const { return _singleShot; }

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
  // Must be exactly 16 hex characters (0-9, a-f, A-F)
  static const QRegularExpression re("^[0-9a-fA-F]{16}$");

  if (!re.match(param).hasMatch()) {
    setStatus("Invalid XOR key: must be exactly 16 hex characters");
    return;
  }

  QByteArray ba = QByteArray::fromHex(param.toUtf8());
  if (_byteMask == ba)
    return;

  _byteMask = ba;
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
