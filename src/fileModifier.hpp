#pragma once
#include <QObject>
#include <qstringview.h>
#include <qtmetamacros.h>

class FileModifier : public QObject {
  Q_OBJECT

public:
  struct Task {
    QString _fromPath{""};
    QString _toPath{""};
    QByteArray _byteMask{QByteArray::fromHex("0011223344556677")};
    int _chunkSizeBytes{256 * 1024}; // FileModifier processes data in chunks
    bool _deleteOnModify{false};
  };

  struct Progress {
    QString _info{""};
    QString _taskName{""};
    int _completePercent{0};
    enum class Status { InProgress, Finished, Failed } _status;
  };

  explicit FileModifier(const Task &task, QObject *parent = nullptr);
  void requestStop();

private:
  const Task _task;
  const QString _taskName;
  std::atomic<bool> _stopRequested;

public slots:
  void onProcess();
signals:
  void progress(const Progress &progress);
  void finished(const Progress &progress);
};
