#pragma once
#include "fileModifier.hpp"
#include <QMutex>
#include <QObject>
#include <QSet>
#include <QTimer>
#include <chrono>
#include <qobject.h>
#include <qstringview.h>
#include <qtmetamacros.h>

class FileScheduler : public QObject {
  Q_OBJECT

public:
  enum class FileRepeatAction { Overwrite, Pass, Copy };
  Q_ENUM(FileRepeatAction)

  struct Task {
    bool _run = false;
    bool _deleteOnModify = false;
    QByteArray _byteMask = QByteArray::fromHex("0011223344556677");
    QString _inputFileMask = "";
    QString _fromDirectory = "";
    QString _toDirectory = "";
    FileRepeatAction _fileRepeatAction = FileRepeatAction::Copy;

    std::chrono::milliseconds _queryInterval = std::chrono::milliseconds{1000};
    bool _singleShot = true;
    int _maxconcurrentProcesses = 10;
    int _chunkSizeBytes = 1024 * 256;
  };
  explicit FileScheduler(const Task &task, QObject *parent = nullptr);
  const Task &getTask() const;
  void setTask(const Task &task);

private:
  void applyTask();
  void processQuery();
  void scheduleModifier(const FileModifier::Task &task);

  Task _task;
  QMutex _taskAccessMutex;
  QTimer *_queryTimer;
  int _runningThreads;
  QSet<QString> _activeFiles;

private slots:
  void onTimer();

public slots:
  void onModifierProgress(const FileModifier::Progress &progress);
  void onModifierFinished(const FileModifier::Progress &progress);

signals:
  void showUserInfo(const QString &info);
};
