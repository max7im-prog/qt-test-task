#pragma once
#include <QObject>
#include <qstringview.h>
#include <qtmetamacros.h>

class FileModifier : public QObject {
  Q_OBJECT

public:
  struct Task {
    QString _fromPath;
    QString _toPath;
    QByteArray _byteMask;
    int _chunkSizeBytes; // FileModifier processes data in chunks
  };

  struct Progress {
    QString _info;
  };

  explicit FileModifier(const Task &task, QObject *parent = nullptr);
  void requestStop();

private:
  const Task _task;
  std::atomic<bool> _stopRequested;
public slots:
  void onProcess();
signals:
  void progress(const Progress &progress);
  void finished(const Progress &progress);
};
