#include "fileModifier.hpp"
#include <QDir>
#include <fstream>
#include <qdebug.h>
#include <qobject.h>

namespace {
static constexpr int s_minChunkSizeBytes = 64;
static constexpr int s_maxChunkSizeBytes = 1024 * 1024; // 1 MB
} // namespace

FileModifier::FileModifier(const Task &task, QObject *parent)
    : QObject(parent), _task(task), _stopRequested(false) {}

void FileModifier::requestStop() { _stopRequested = true; }

void FileModifier::onProcess() {

  qInfo().noquote() << "processing" << "\n";
  emit progress({._info = "Started modifier"});

  QFile fromFile(_task._fromPath);
  QFile toFile(_task._toPath);

  if (!fromFile.open(QIODevice::ReadOnly)) {
    emit finished({._info = "Failed to open input file: " + _task._fromPath});
    return;
  }

  if (!toFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    emit finished({._info = "Failed to open output file: " + _task._toPath});
    return;
  }

  if (_task._byteMask.size() != 8) {
    emit finished({._info = "Byte mask of size " +
                            QString::number(_task._byteMask.size()) +
                            ", 8 required"});
    return;
  }

  int chunkSize = qMin(qMax(s_minChunkSizeBytes, _task._chunkSizeBytes),
                       s_maxChunkSizeBytes);

  QByteArray fileBuffer;
  fileBuffer.resize(chunkSize);
  for (int filePos = 0; !fromFile.atEnd() && !_stopRequested;) {

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

  emit finished(
      {._info = "Modified " + _task._fromPath + " >> " + _task._toPath});
}
