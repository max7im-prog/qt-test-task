#include "fileModifier.hpp"
#include <QDir>
#include <qdebug.h>
#include <qobject.h>

namespace {
static constexpr int s_minChunkSizeBytes = 64;
static constexpr int s_maxChunkSizeBytes = 1024 * 1024; // 1 MB
} // namespace

FileModifier::FileModifier(const Task &task, QObject *parent)
    : QObject(parent), _task(task), _stopRequested(false),
      _taskName(_task._fromPath) {}

void FileModifier::requestStop() { _stopRequested = true; }

void FileModifier::onProcess() {

  emit progress({._info = "Started modifier",
                 ._taskName = _taskName,
                 ._completePercent = 0,
                 ._status = Progress::Status::InProgress});

  QFile fromFile(_task._fromPath);
  QFile toFile(_task._toPath);

  if (!fromFile.open(QIODevice::ReadOnly)) {
    emit finished({._info = "Failed to open input file: " + _task._fromPath,
                   ._taskName = _taskName,
                   ._completePercent = 0,
                   ._status = Progress::Status::Failed});
    return;
  }

  if (!toFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    emit finished({._info = "Failed to open output file: " + _task._toPath,
                   ._taskName = _taskName,
                   ._completePercent = 0,
                   ._status = Progress::Status::Failed});
    return;
  }

  if (_task._byteMask.size() != 8) {
    emit finished({._info = "Byte mask of size " +
                            QString::number(_task._byteMask.size()) +
                            ", 8 required",
                   ._taskName = _taskName,
                   ._completePercent = 0,
                   ._status = Progress::Status::Failed});
    return;
  }

  int chunkSize = qMin(qMax(s_minChunkSizeBytes, _task._chunkSizeBytes),
                       s_maxChunkSizeBytes);

  QByteArray fileBuffer;
  fileBuffer.resize(chunkSize);
  for (int filePos = 0; !fromFile.atEnd() && !_stopRequested;) {
    emit progress({._info = "Progress",
                   ._taskName = _taskName,
                   ._completePercent = static_cast<int>(
                       100.0f * static_cast<float>(filePos) / fromFile.size()),
                   ._status = Progress::Status::InProgress});

    // Read data
    int bytesRead = fromFile.read(fileBuffer.data(), fileBuffer.size());

    // Apply  XOR
    for (int chunkPos = 0; chunkPos < bytesRead; ++chunkPos) {
      fileBuffer[chunkPos] ^=
          _task._byteMask.at((filePos++) % _task._byteMask.size());
    }

    // Write data
    toFile.write(fileBuffer.constData(), bytesRead);
  }
  if (!fromFile.atEnd()) {
    emit finished(
        {._info = "Aborted " + _task._fromPath + " >> " + _task._toPath,
         ._taskName = _taskName,
         ._completePercent = 0,
         ._status = Progress::Status::Failed});
    return;
  }

  if (_task._deleteOnModify) {
    fromFile.close();
    if (fromFile.exists()) {

      fromFile.remove();
    }
  }
  emit finished(
      {._info = "Modified " + _task._fromPath + " >> " + _task._toPath,
       ._taskName = _taskName,
       ._completePercent = 100,
       ._status = Progress::Status::Finished});
}
