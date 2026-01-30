#pragma once
#include "fileScheduler.hpp"
#include "taskModel.hpp"
#include <chrono>
#include <qdir.h>
#include <qobject.h>
#include <qtmetamacros.h>

class AppController : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString status READ getStatus WRITE setStatus NOTIFY statusChanged)
  Q_PROPERTY(QString log READ getLog NOTIFY logChanged)
  Q_PROPERTY(TaskModel *tasks READ getTasks CONSTANT)

  Q_PROPERTY(QString inputDir READ getInputDir WRITE setInputDir NOTIFY
                 inputDirChanged)
  Q_PROPERTY(QString outputDir READ getOutputDir WRITE setOutputDir NOTIFY
                 outputDirChanged)
  Q_PROPERTY(QString fileMask READ getFileMask WRITE setFileMask NOTIFY
                 fileMaskChanged)
  Q_PROPERTY(QString byteMask READ getByteMask WRITE setByteMask NOTIFY
                 byteMaskChanged)
  Q_PROPERTY(int queryIntervalMs READ getQueryIntervalMs WRITE
                 setQueryIntervalMs NOTIFY queryIntervalMsChanged)
  Q_PROPERTY(bool singleShot READ getSingleShot WRITE setSingleShot NOTIFY
                 singleShotChanged)
  Q_PROPERTY(bool deleteOnModify READ getDeleteOnModify WRITE setDeleteOnModify
                 NOTIFY deleteOnModifyChanged)
  Q_PROPERTY(FileScheduler::FileRepeatAction repeatAction READ getRepeatAction
                 WRITE setRepeatAction NOTIFY repeatActionChanged)
  Q_PROPERTY(
      bool running READ getRunning WRITE setRunning NOTIFY runningChanged)

public:
  explicit AppController(QObject *parent = nullptr);

  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

  const QString &getStatus() const;
  const QString &getInputDir() const;
  const QString &getOutputDir() const;
  const QString &getFileMask() const;
  const QString &getByteMask() const;
  int getQueryIntervalMs() const;
  bool getSingleShot() const;
  const QString &getLog() const;
  bool getDeleteOnModify() const;
  FileScheduler::FileRepeatAction getRepeatAction() const;
  TaskModel *getTasks();
  bool getRunning() const;

  void setStatus(const QString &param);
  void setInputDir(const QString &param);
  void setOutputDir(const QString &param);
  void setFileMask(const QString &param);
  void setByteMask(const QString &param);
  void setQueryIntervalMs(int param);
  void setSingleShot(bool param);
  void appendLog(const QString &msg);
  void setDeleteOnModify(bool param);
  void setRepeatAction(FileScheduler::FileRepeatAction param);
  void setRunning(bool param);

signals:
  void logChanged();
  void statusChanged();
  void inputDirChanged();
  void outputDirChanged();
  void fileMaskChanged();
  void byteMaskChanged();
  void queryIntervalMsChanged();
  void singleShotChanged();
  void deleteOnModifyChanged();
  void repeatActionChanged();
  void runningChanged();

private:
  FileScheduler _scheduler;
  QString _status{"Stopped"};
  QString _inputDir{"./input"};
  QString _outputDir{"./output"};
  QString _fileMask{"*.txt"};
  QString _byteMask{"0011223344556677"};
  int _queryIntervalMs{1000};
  bool _singleShot{false};
  QString _log{""};
  FileScheduler::FileRepeatAction _fileRepeatAction{
      FileScheduler::FileRepeatAction::Copy};
  bool _deleteOnModify{false};

  TaskModel *const _taskModel;
  bool _running{false};
};
