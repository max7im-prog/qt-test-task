
#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>

class TaskModel : public QAbstractListModel {
  Q_OBJECT

public:
  enum Roles { NameRole = Qt::UserRole + 1, ProgressRole, StatusRole };
  Q_ENUM(Roles)

  explicit TaskModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void addTask(const QString &name);
  Q_INVOKABLE void updateProgress(const QString &name, int progress,
                                  const QString &status = QString());
  Q_INVOKABLE void finishTask(const QString &name);
  Q_INVOKABLE void clear();

private:
  struct Task {
    QString name;
    int progress = 0;
    QString status;
  };

  int findTaskIndex(const QString &name) const;

  QVector<Task> m_tasks;
};
