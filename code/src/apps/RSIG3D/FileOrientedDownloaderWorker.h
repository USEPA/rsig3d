#include <QThread>
#include <QListWidget>

class FileOrientedDownloaderWorker : public QThread {
  Q_OBJECT
    
  signals:
  void fileDownloaded();
  void finished(bool);
  void numFilesSignal(long numFiles);

public:
  FileOrientedDownloaderWorker() : listWidget( 0 ), abortFlag( false ) {}
  ~FileOrientedDownloaderWorker() { /* LEAK BUG? */ /* delete listWidget; */ }
  void setQListWidget(const QListWidget * widget);
  void setBaseDir(QString dir);
  void setAbortFlag(bool flag);
  
private:
  const QListWidget* listWidget;
  QString baseDir;
  bool abortFlag;
  void run();
};
