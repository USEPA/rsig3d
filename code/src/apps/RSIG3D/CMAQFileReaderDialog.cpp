
/******************************************************************************
PURPOSE: CMAQFileReaderDialog.cpp - Implements CMAQFileReaderDialog for a
         CMAQFileReaderModel.

NOTES:   Uses Qt GUI Library.

HISTORY: 2022-01-10 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <string> // For std::string.

#include <QApplication>
#include <QString>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QRegExp>
#include <QRegExpValidator>
#include <QPushButton>
#include <QMessageBox>

#include <Assertions.h> // For PRE(), POST(), CHECK(), DEBUG().
#include <DataUtilities.h>  // For emvl::isDirectory().

#include "CMAQFileReaderModel.h"  // For class CMAQFileReaderModel.
#include "ui_cmaqfilereader.h"    // For class Ui_CMAQFileReader.
#include "CMAQFileReaderDialog.h" // For class CMAQFileReaderDialog.

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: CMAQFileReaderDialog - Constructor.
INPUTS:  CMAQFileReaderModel& model  The CMAQFileReaderModel to show/update.
******************************************************************************/

CMAQFileReaderDialog::CMAQFileReaderDialog( CMAQFileReaderModel& theModel ) :
  QDialog( 0 ), ui( 0 ), model( theModel ) {

  PRE0( model.invariant() );
  model.clearInputFiles(); // Clear input data files and data variable.
  model.clearOutputFile();

  // QT magic:

  ui = new Ui_CMAQFileReader;
  ui->setupUi( this ); // What does this do?
  ui->dataDirectoryGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->dataFileGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->dataVariableGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->htDirectoryGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->htDirectoryListingGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->zfDirectoryGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->zfDirectoryListingGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->wwindDirectoryGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->wwindDirectoryListingGroupbox->setStyleSheet("QGroupBox {border:0;}");
  ui->groupBox_localremote->setStyleSheet("QGroupBox {border:0;}");

  // Finish setting up ui widgets:

  const int windowX = 100;
  const int windowY = 30;
  const int windowWidth = 750; // No reason to make it smaller or larger.
  const int minimumWindowHeight = 730; // Max height that fits a 1024x768 laptop
  const int maximumWindowHeight = 1050; // No reason to make it any taller.
  const char* const regex    = "[\\w-\\.]*";
  const char* const regexDir = "[\\w-\\\\./: ]*";
  const char* const name = "CMAQ File Reader Dialog";

  setModal( true ); // Block application until this dialog is closed.
  setWindowTitle( QApplication::translate( name, name, 0,
                                           QApplication::UnicodeUTF8 ) );
  setMinimumWidth( windowWidth ); // Fixed-width fits on laptops.
  setMaximumWidth( windowWidth ); // No use resizing width.
  setMinimumHeight( minimumWindowHeight ); // Fits on laptops.
  setMaximumHeight( maximumWindowHeight ); // No use making it taller.
  setGeometry( windowX, windowY, windowWidth, minimumWindowHeight );

  const int isLocal = model.getMode() == CMAQFileReaderModel::LOCAL;

  // Local:

  ui->localButton->setToolTip( QString::fromUtf8(
    "Read CMAQ files on this computer." ) );
  ui->localButton->setChecked( isLocal );
  connect( ui->localButton, SIGNAL( clicked() ), this, SLOT( onLocal()));


  // Remote:

  ui->remoteButton->setToolTip( QString::fromUtf8(
    "Read CMAQ files on a remote networked computer." ) );
  ui->remoteButton->setChecked( ! isLocal );
  connect( ui->remoteButton, SIGNAL( clicked() ), this, SLOT( onRemote()));


  // User:

  ui->userText->setVisible( false );
  ui->userText->setToolTip( QString::fromUtf8(
    "Login name on remote host (e.g., mfreem02)" ) );
  ui->userText->setMaxLength( 32 );
  ui->userText->setFixedWidth( 100 );
  ui->userText->setValidator( new QRegExpValidator( QRegExp( regex )));
  const std::string user( model.getUser() );
  ui->userText->setText( QApplication::translate( name, user.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  connect( ui->userText, SIGNAL( editingFinished() ), this, SLOT( onSetUser()));


  // Host:

  ui->hostText->setVisible( false );
  ui->hostText->setToolTip( QString::fromUtf8(
    "Name of remote host (e.g. atmos1.hesc.epa.gov)" ) );
  ui->hostText->setMaxLength( 64 );
  ui->hostText->setFixedWidth( 200 );
  ui->hostText->setValidator( new QRegExpValidator( QRegExp( regex )));
  const std::string host( model.getHost() );
  ui->hostText->setText( QApplication::translate( name, host.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );


  // Connect:

  ui->connectButton->setToolTip( QString::fromUtf8(
    "Read CMAQ files on a remote networked computer." ) );
  ui->connectButton->setVisible( false );
  ui->connectButton->setDefault( false ); // Ignore return/enter key presses.
  ui->connectButton->setAutoDefault( false ); // Ignore return/enter key presses
  connect( ui->connectButton, SIGNAL( clicked() ), this, SLOT( onConnect()));



  // Data Directory text:

  ui->dataDirectoryText->setToolTip( QString::fromUtf8(
                                                  "Data directory on host." ));
  const std::string dataDirectory( model.getDataDirectory() );
  ui->dataDirectoryText->setText( QApplication::translate( name,
                                                          dataDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  ui->dataDirectoryText->setMaxLength( 256 );
  ui->dataDirectoryText->setValidator( new QRegExpValidator(QRegExp(regexDir)));
  connect( ui->dataDirectoryText, SIGNAL( editingFinished() ), this,
           SLOT( onSetDataDirectory() ) );


  // Data Directory Listing text:

  ui->dataDirectoryListingText->setToolTip( QString::fromUtf8(
                              "Select a subdirectory or data file."));
  {
    const std::string dataDirectoryListing( model.getDataDirectoryListing() );
    QString listing( dataDirectoryListing.c_str() ); //x
    QStringList list( listing.split( QRegExp( "[\r\n]" ),
                                     QString::SkipEmptyParts ) ); //x
    ui->dataDirectoryListingText->addItems( list );
  }

  connect( ui->dataDirectoryListingText,
           SIGNAL( itemClicked(QListWidgetItem*) ), this,
           SLOT( onSelectedDataDirectoryListingItem( QListWidgetItem* )));


  // Data Variable Listing text:

  ui->dataVariableListingText->setToolTip( QString::fromUtf8(
                                              "Select a variable to read." ) );

  if ( ! model.getDataVariable().empty() ) {
    const std::string dataVariableListing( model.getDataVariableListing() );
    QString listing( dataVariableListing.c_str() ); //x
    QStringList list( listing.split( QRegExp( "[\r\n]" ),
                                     QString::SkipEmptyParts ) ); //x
    ui->dataVariableListingText->addItems( list );
  }

  connect( ui->dataVariableListingText,
           SIGNAL( itemClicked(QListWidgetItem*) ), this,
           SLOT( onSelectedDataVariable( QListWidgetItem* ) ) );


  // HT Directory text:

  ui->htDirectoryText->setToolTip( QString::fromUtf8( "Directory containing "
    "Optional: GRIDCRO2D file (with HT variable) for time-varying elevations."));
  const std::string htDirectory( model.getHTDirectory() );
  ui->htDirectoryText->setText( QApplication::translate( name,
                                                        htDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  ui->htDirectoryText->setMaxLength( 256 );
  ui->htDirectoryText->setValidator( new QRegExpValidator( QRegExp(regexDir)));
  connect( ui->htDirectoryText, SIGNAL( editingFinished() ), this,
           SLOT( onSetHTDirectory() ) );


  // HT Directory Listing text:

  ui->htDirectoryListingText->setToolTip( QString::fromUtf8(
    "Optional: Select a subdirectory or GRIDCRO2D file (containing HT) "
    "for time-varying elevations." ) );
  {
    const std::string htDirectoryListing( model.getHTDirectoryListing() );
    QString listing( htDirectoryListing.c_str() ); //x
    QStringList list( listing.split( QRegExp( "[\r\n]" ),
                                     QString::SkipEmptyParts ) ); //x
    ui->htDirectoryListingText->addItems( list );
  }

  connect( ui->htDirectoryListingText,
           SIGNAL( itemClicked(QListWidgetItem*) ), this,
           SLOT( onSelectedHTDirectoryListingItem( QListWidgetItem* ) ) );


  // ZF Directory text:

  ui->zfDirectoryText->setToolTip( QString::fromUtf8( "Directory containing "
    "Optional: METCRO3D file (with ZF variable) for time-varying elevations."));
  const std::string zfDirectory( model.getZFDirectory() );
  ui->zfDirectoryText->setText( QApplication::translate( name,
                                                        zfDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  ui->zfDirectoryText->setMaxLength( 256 );
  ui->zfDirectoryText->setValidator( new QRegExpValidator( QRegExp(regexDir)));
  connect( ui->zfDirectoryText, SIGNAL( editingFinished() ), this,
           SLOT( onSetZFDirectory() ) );


  // ZF Directory Listing text:

  ui->zfDirectoryListingText->setToolTip( QString::fromUtf8(
    "Optional: Select a subdirectory or METCRO3D file (containing ZF) "
    "for time-varying elevations." ) );
  {
    const std::string zfDirectoryListing( model.getZFDirectoryListing() );
    QString listing( zfDirectoryListing.c_str() ); //x
    QStringList list( listing.split( QRegExp( "[\r\n]" ),
                                     QString::SkipEmptyParts ) ); //x
    ui->zfDirectoryListingText->addItems( list );
  }

  connect( ui-> zfDirectoryListingText,
           SIGNAL( itemClicked(QListWidgetItem*) ), this,
           SLOT( onSelectedZFDirectoryListingItem( QListWidgetItem* ) ) );


  // WWIND Directory text:

  ui->wwindDirectoryText->setToolTip( QString::fromUtf8( "Optional: "
    "Directory containing METCRO3D/CONC file (containing WWIND or W_VEL) "
    "for 3D WIND vectors." ) );
  const std::string wwindDirectory( model.getWWINDDirectory() );
  ui->wwindDirectoryText->setText( QApplication::translate( name,
                                                    wwindDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  ui->wwindDirectoryText->setMaxLength( 256 );
  ui->wwindDirectoryText->setValidator(new QRegExpValidator(QRegExp(regexDir)));
  connect( ui->wwindDirectoryText, SIGNAL( editingFinished() ), this,
           SLOT( onSetWWINDDirectory() ) );


  // WWIND Directory Listing text:

  ui->wwindDirectoryListingText->setToolTip( QString::fromUtf8(
    "Optional: Select a subdirectory or METCRO3D/CONC file "
    "(containing WWIND or W_VEL) for 3D WIND vectors." ) );
  {
    const std::string wwindDirectoryListing( model.getWWINDDirectoryListing() );
    QString listing( wwindDirectoryListing.c_str() ); //x
    QStringList list( listing.split( QRegExp( "[\r\n]" ),
                                     QString::SkipEmptyParts ) ); //x
    ui->wwindDirectoryListingText->addItems( list );
  }

  connect( ui->wwindDirectoryListingText,
           SIGNAL( itemClicked(QListWidgetItem*) ), this,
           SLOT( onSelectedWWINDDirectoryListingItem( QListWidgetItem* ) ) );


  // Read button:

  ui->readButton->setToolTip( QString::fromUtf8(
    "Run CMAQSubset on files and read subsetted variable data." ) );
  ui->readButton->setDefault( false ); // Ignore return/enter key presses.
  ui->readButton->setAutoDefault( false ); // Ignore return/enter key presses.
  connect( ui->readButton, SIGNAL( clicked() ), this, SLOT( onReadVariable()));

  // Cancel button:

  ui->cancelButton->setDefault( false ); // Ignore return/enter key presses.
  ui->cancelButton->setAutoDefault( false ); // Ignore return/enter key presses.
  connect( ui->cancelButton, SIGNAL( clicked() ), this, SLOT( onCancel() ) );

  if ( isLocal ) {
    onLocal();
  } else {
    onRemote();
    onConnect(); // Connect automatically if host is specified in the model.
  }

  // Note parent window deletes child widgets created above when destructed.

  POST( true );
}



/******************************************************************************
PURPOSE: ~CMAQFileReaderDialog - Destructor.
******************************************************************************/

CMAQFileReaderDialog::~CMAQFileReaderDialog() {
  delete ui, ui = 0;
}



/******************************************************************************
PURPOSE: invariant - Is object in a valid state?
RETURNS: bool true or else there is a defect in the program.
NOTES: Called implicitly by PRE() and POST() unless compiled with
       -DNO_ASSERTIONS.
******************************************************************************/

bool CMAQFileReaderDialog::invariant() const {
  const bool result =
    AND20( model.invariant(),
           ui,
           ui->localButton,
           ui->remoteButton,
           ui->connectButton,
           ui->userLabel,
           ui->userText,
           ui->hostLabel,
           ui->hostText,
           ui->dataDirectoryText,
           ui->dataDirectoryListingText,
           ui->dataVariableListingText,
           ui->htDirectoryText,
           ui->htDirectoryListingText,
           ui->zfDirectoryText,
           ui->zfDirectoryListingText,
           ui->wwindDirectoryText,
           ui->wwindDirectoryListingText,
           ui->readButton,
           ui->cancelButton );
  return result;
}



//============================ PRIVATE FUNCTIONS ==============================



/******************************************************************************
PURPOSE: onLocal - Called when Local button is clicked.
         It hides userText, hostText and connectButton and
         clears the model user and host.
******************************************************************************/

void CMAQFileReaderDialog::onLocal() {
  ui->userLabel->setVisible( false );
  ui->userText->setVisible( false );
  ui->hostLabel->setVisible( false );
  ui->hostText->setVisible( false );
  ui->connectButton->setVisible( false );
  model.setMode( CMAQFileReaderModel::LOCAL );
  updateFromModel();
}



/******************************************************************************
PURPOSE: onRemote - Called when Remote button is clicked.
         It shows userText, hostText and connectButton and
         clears the model user and host.
******************************************************************************/

void CMAQFileReaderDialog::onRemote() {
  ui->userLabel->setVisible( true );
  ui->userText->setVisible( true );
  ui->hostLabel->setVisible( true );
  ui->hostText->setVisible( true );
  ui->connectButton->setVisible( true );
  model.setMode( CMAQFileReaderModel::REMOTE );
  updateFromModel();
}



/******************************************************************************
PURPOSE: onSetUser - Called when userText is set.
******************************************************************************/

void CMAQFileReaderDialog::onSetUser() {
  model.setUser( ui->userText->text().toStdString() );
}



/******************************************************************************
PURPOSE: onConnect - Called when Connect button is clicked.
******************************************************************************/

void CMAQFileReaderDialog::onConnect() {
  const std::string& host = ui->hostText->text().toStdString();

  if ( AND2( model.getMode() == CMAQFileReaderModel::REMOTE, ! host.empty())) {

    /*
     * model.setHost() is an expensive call so only do it if host has changed.
     * It queries the remote host platform and then tries to install CMAQSubset
     * on the remote host.
     * It can fail for various reasons beyond this program's control.
     * If it fails, the user will have to manually setup ssh and copy CMAQSubset
     * to the remote host $HOME/.rsig/CMAQSubset.
     */

    if ( host != model.getHost() ) {
      disableWhileConnecting();

      try {
        model.setHost( host ); // This is an expensive call that takes a minute.
      } catch ( ... ) {
      }

      enableAfterConnect();

      /*
       * Check if host change was successful.
       * If it failed then pop-up a warning telling the user that it failed and
       * suggesting a course of action they should take to remedy the problem.
       */

      if ( model.getHost() != host ) { // Failed to change host.
        const char* const failureMessageString =
          "I'm sorry, unable to ssh to the host and install the required "
          "CMAQSubset program.\n\n"
          "To diagnose the problem try the following:\n"
          "\n(1). Make sure your computer has an active Internet connection."
          "Open a web browser to https://www.epa.gov/rsig\n"
          "\n(2). Open a Shell/Terminal/Command window and test access to the "
          "remote host using ssh.\nFor example:\n"
          "  ssh fred@atmos1.hesc.epa.gov ls\n"
          "\n(3). If that fails, then ask your system administrator for help "
          "in setting up ssh with an empty passphrase.\n"
          "\n(4). Once ssh is working, copy the CMAQSubset program from "
          "the RSIG3D/bin/$platform/ subdirectory on this computer to your "
          "$HOME/.rsig/ "
          "subdirectory on the remote host computer. For example:\n\n"
          "  ssh fred@atmos1.hesc.epa.gov mkdir .rsig\n\n"
          "  cd $HOME/RSIG3D/bin/Linux.x86_64\n\n"
          "  scp -p CMAQSubset fred@atmos1.hesc.epa.gov:.rsig/\n\n"
          "  ssh fred@atmos1.hesc.epa.gov .rsig/CMAQSubset\n"
          "\n(5). Once CMAQSubset can be run on the remote host using ssh "
          "you can try re-entering the host name\n"
          "(and your user name on the remote host\n"
          "if it differs from your user name on the local host).\n"
          "\nIf problems persist contact us at rsig@epa.gov.\n";
        const char* const name = "failureMessage";
        QMessageBox::warning( this, tr( name ),
                              tr( failureMessageString ),
                              QMessageBox::Cancel );
        ui->hostText->clear(); // Erase the failed remote host name attempted.
      } else { // Model state changed to new host so update GUI to show changes:
        updateFromModel();
      }
    }
  }
}



/******************************************************************************
PURPOSE: disableWhileConnecting - Disable certain widgets while connecting.
******************************************************************************/

void CMAQFileReaderDialog::disableWhileConnecting() {
  QApplication::setOverrideCursor( Qt::WaitCursor ); // Hourglass or spinning.
  ui->connectButton->setEnabled( false );
  ui->localButton->setEnabled( false );
  ui->remoteButton->setEnabled( false );
  ui->userText->setEnabled( false );
  ui->hostText->setEnabled( false );
  ui->dataDirectoryText->setEnabled( false );
  ui->htDirectoryText->setEnabled( false );
  ui->zfDirectoryText->setEnabled( false );
  ui->wwindDirectoryText->setEnabled( false );
  ui->readButton->setEnabled( false );
}



/******************************************************************************
PURPOSE: enableAfterConnect - Enable disabled widgets after connect attempt.
******************************************************************************/

void CMAQFileReaderDialog::enableAfterConnect() {
  ui->connectButton->setEnabled( true );
  ui->localButton->setEnabled( true );
  ui->remoteButton->setEnabled( true );
  ui->userText->setEnabled( true );
  ui->hostText->setEnabled( true );
  ui->dataDirectoryText->setEnabled( true );
  ui->htDirectoryText->setEnabled( true );
  ui->zfDirectoryText->setEnabled( true );
  ui->wwindDirectoryText->setEnabled( true );
  ui->readButton->setEnabled( true );
  QApplication::restoreOverrideCursor(); // Normal mouse cursor.
}



/******************************************************************************
PURPOSE: onSetDataDirectory - Called when dataDirectoryText is entered.
******************************************************************************/

void CMAQFileReaderDialog::onSetDataDirectory() {
  updateDirectory( &CMAQFileReaderModel::setDataDirectory,
                   &CMAQFileReaderModel::getDataDirectoryListing,
                   ui->dataDirectoryText,
                   ui->dataDirectoryListingText );
}



/******************************************************************************
PURPOSE: onSetHTDirectory - Called when htDirectoryText is entered.
******************************************************************************/

void CMAQFileReaderDialog::onSetHTDirectory() {
  updateDirectory( &CMAQFileReaderModel::setHTDirectory,
                   &CMAQFileReaderModel::getHTDirectoryListing,
                   ui->htDirectoryText,
                   ui->htDirectoryListingText );
}



/******************************************************************************
PURPOSE: onSetZFDirectory - Called when zfDirectoryText is entered.
******************************************************************************/

void CMAQFileReaderDialog::onSetZFDirectory() {
  updateDirectory( &CMAQFileReaderModel::setZFDirectory,
                   &CMAQFileReaderModel::getZFDirectoryListing,
                   ui->zfDirectoryText,
                   ui->zfDirectoryListingText );
}



/******************************************************************************
PURPOSE: onSetWWINDDirectory - Called when wwindDirectoryText is entered.
******************************************************************************/

void CMAQFileReaderDialog::onSetWWINDDirectory() {
  updateDirectory( &CMAQFileReaderModel::setWWINDDirectory,
                   &CMAQFileReaderModel::getWWINDDirectoryListing,
                   ui->wwindDirectoryText,
                   ui->wwindDirectoryListingText );
}



/******************************************************************************
PURPOSE: onSelectedDataDirectoryListingItem - Called when dataDirectoryListing
         item (subdirectory or file) is selected.
INPUTS:  QListWidgetItem* entry  Selected directory listing item.
******************************************************************************/

void CMAQFileReaderDialog::onSelectedDataDirectoryListingItem(
                                                    QListWidgetItem* item ) {

  updateListing( item,
                 &CMAQFileReaderModel::getDataDirectory,
                 &CMAQFileReaderModel::setDataDirectory,
                 &CMAQFileReaderModel::getDataDirectoryListing,
                 &CMAQFileReaderModel::setDataFile,
                 &CMAQFileReaderModel::getDataVariableListing,
                 ui->dataDirectoryText,
                 ui->dataDirectoryListingText,
                 ui->dataVariableListingText );

}



/******************************************************************************
PURPOSE: onSelectedHTDirectoryListingItem - Called when htDirectoryListing
         item (subdirectory or file) is selected.
INPUTS:  QListWidgetItem* entry  Selected directory listing item.
******************************************************************************/

void CMAQFileReaderDialog::onSelectedHTDirectoryListingItem(
                                                    QListWidgetItem* item ) {

  updateListing( item,
                 &CMAQFileReaderModel::getHTDirectory,
                 &CMAQFileReaderModel::setHTDirectory,
                 &CMAQFileReaderModel::getHTDirectoryListing,
                 &CMAQFileReaderModel::setHTFile,
                 0,
                 ui->htDirectoryText,
                 ui->htDirectoryListingText,
                 0 );
}



/******************************************************************************
PURPOSE: onSelectedZFDirectoryListingItem - Called when zfDirectoryListing
         item (subdirectory or file) is selected.
INPUTS:  QListWidgetItem* item  Selected directory listing item.
******************************************************************************/

void CMAQFileReaderDialog::onSelectedZFDirectoryListingItem(
                                                    QListWidgetItem* item ) {

  updateListing( item,
                 &CMAQFileReaderModel::getZFDirectory,
                 &CMAQFileReaderModel::setZFDirectory,
                 &CMAQFileReaderModel::getZFDirectoryListing,
                 &CMAQFileReaderModel::setZFFile,
                 0,
                 ui->zfDirectoryText,
                 ui->zfDirectoryListingText,
                 0 );
}



/******************************************************************************
PURPOSE: onSelectedWWINDDirectoryListingItem - Called when wwindDirectoryListing
         item (subdirectory or file) is selected.
INPUTS:  QListWidgetItem* item  Selected directory listing item.
******************************************************************************/

void CMAQFileReaderDialog::onSelectedWWINDDirectoryListingItem(
                                                    QListWidgetItem* item ) {

  updateListing( item,
                 &CMAQFileReaderModel::getWWINDDirectory,
                 &CMAQFileReaderModel::setWWINDDirectory,
                 &CMAQFileReaderModel::getWWINDDirectoryListing,
                 &CMAQFileReaderModel::setWWINDFile,
                 0,
                 ui->wwindDirectoryText,
                 ui->wwindDirectoryListingText,
                 0 );
}



/******************************************************************************
PURPOSE: updateDirectory - Helper for when directory text is entered.
******************************************************************************/

void CMAQFileReaderDialog::updateDirectory( Setter setDirectory,
                                            Lister getDirectoryListing,
                                            QLineEdit* directoryText,
                                            QListWidget* directoryListingText) {
  PRE4( setDirectory,
        getDirectoryListing,
        directoryText,
        directoryListingText );

  const QString text( directoryText->text() );

  if ( ! text.isEmpty() ) {
    std::string directory( text.toStdString() );
    emvl::trimSlashes( directory );

    if ( OR2( model.getMode() == CMAQFileReaderModel::REMOTE,
              emvl::isDirectory( directory ) ) ) {
      QApplication::setOverrideCursor( Qt::WaitCursor ); // Hourglass.
      (model.*setDirectory)( directory );
      updateFromModel();
      updateListing( getDirectoryListing, directoryListingText );
      QApplication::restoreOverrideCursor(); // Normal mouse cursor.
    }
  }
}



/******************************************************************************
PURPOSE: updateListing - Update a DirectoryListing.
INPUTS:  Lister getDirectoryListing        Model Getter for directory listing.
         Lister getVariableListing         0 or Model Getter for variable list.
OUTPUTS: QListWidget* directoryListingText Directory listing text to update.
******************************************************************************/

void CMAQFileReaderDialog::updateListing( Lister getListing,
                                          QListWidget* listingText ) {

  PRE2( getListing, listingText );

  const std::string modelListing( (model.*getListing)() );
  QString listing( modelListing.c_str() ); //x
  QStringList list( listing.split( QRegExp( "[\r\n]" ),
                                   QString::SkipEmptyParts ) ); //x
  listingText->clear();
  listingText->addItems( list );
}



/******************************************************************************
PURPOSE: updateListing - Update a DirectoryListing when an item (subdirectory
         or file) is selected.
INPUTS:  const QListWIdgetItem* item       Selected directory listing item.
         Getter getDirectory               Model getter for directory name.
         Setter setDirectory               Model setter for directory name.
         Lister getDirectoryListing        Model Getter for directory listing.
         Setter setFile                    0 or Model setter for file name.
         Lister getVariableListing         0 or Model Getter for variable list.
OUTPUTS: QLineEdit* directoryText          Directory text to update.
         QListWidget* directoryListingText Directory listing text to update.
         QListWidget* variableListingText  0 or Variable listing text to update.
******************************************************************************/

void CMAQFileReaderDialog::updateListing( const QListWidgetItem* item,
                                          Getter getDirectory,
                                          Setter setDirectory,
                                          Lister getDirectoryListing,
                                          Setter setFile,
                                          Lister getVariableListing,
                                          QLineEdit* directoryText,
                                          QListWidget* directoryListingText,
                                          QListWidget* variableListingText ) {

  PRE5( getDirectory,
        setDirectory,
        getDirectoryListing,
        setFile,
        IMPLIES_ELSE( getVariableListing,
                      variableListingText,
                      ! variableListingText ) );

  if ( item ) {
    const QString itemText( item->text() );

    if ( ! itemText.isEmpty() ) {
      QApplication::setOverrideCursor( Qt::WaitCursor ); // Hourglass.

      if ( itemText.startsWith( ".." ) ) { // Go up directory level if not top:
        const std::string directory( (model.*getDirectory)() ); //x
        const std::string parentDirectory( emvl::parentDirectory( directory ));

        if ( parentDirectory != directory ) { // Not already at the top:
          (model.*setDirectory)( parentDirectory );
          const std::string modelDirectory( (model.*getDirectory)() );
          directoryText->setText( modelDirectory.c_str() );
          updateListing( getDirectoryListing, directoryListingText );
        }

        if ( variableListingText ) {
          variableListingText->clear();
        }
      } else if ( OR2( itemText.endsWith( '/' ), itemText.endsWith( '\\' ))) {
        const std::string directory( (model.*getDirectory)() ); //x
        const std::string subDirectory( itemText.toStdString(), 0,
                                        itemText.length() - 1 );
        const std::string newDirectory( emvl::appendPath( directory,
                                                          subDirectory ) );
        (model.*setDirectory)( newDirectory );
        directoryText->setText( newDirectory.c_str() );
        updateListing( getDirectoryListing, directoryListingText );

        if ( variableListingText ) {
          variableListingText->clear();
        }
      } else {
        (model.*setFile)( itemText.toStdString() );

        if ( variableListingText && getVariableListing ) {
          updateListing( getVariableListing, variableListingText );
        }
      }

      QApplication::restoreOverrideCursor(); // Normal mouse cursor.
    }
  }
}



/******************************************************************************
PURPOSE: updateFromModel - Check model state and update the GUI to reflect the
         model's current state.
******************************************************************************/

void CMAQFileReaderDialog::updateFromModel() {

  QApplication::setOverrideCursor( Qt::WaitCursor ); // Hourglass.

  const char* const context = "CMAQFileReaderDialog";

  const std::string modelUser = model.getUser();
  const std::string guiUser = ui->userText->text().toStdString();

  if ( modelUser != guiUser ) {
    ui->userText->setText( QApplication::translate( context,
                                            modelUser.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  }

  const std::string modelHost = model.getHost();
  const std::string guiHost = ui->hostText->text().toStdString();

  if ( modelHost != guiHost ) {
    ui->hostText->setText( QApplication::translate( context,
                                            modelHost.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );
  }

  const std::string modelDataDirectory = model.getDataDirectory();
  const std::string guiDataDirectory =
    ui->dataDirectoryText->text().toStdString();

  if ( modelDataDirectory != guiDataDirectory ) {
    ui->dataDirectoryText->setText( QApplication::translate( context,
                                            modelDataDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );

    updateListing( &CMAQFileReaderModel::getDataDirectoryListing,
                   ui->dataDirectoryListingText );

    if ( model.getDataFile().empty() ) {
      ui->dataVariableListingText->clear();
    } else {

      updateListing( &CMAQFileReaderModel::getDataVariableListing,
                     ui->dataVariableListingText );
    }
  }

  const std::string modelHTDirectory = model.getHTDirectory();
  const std::string guiHTDirectory = ui->htDirectoryText->text().toStdString();

  if ( modelHTDirectory != guiHTDirectory ) {
    ui->htDirectoryText->setText( QApplication::translate( context,
                                            modelHTDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );

    updateListing( &CMAQFileReaderModel::getHTDirectoryListing,
                   ui->htDirectoryListingText );
  }

  const std::string modelZFDirectory = model.getZFDirectory();
  const std::string guiZFDirectory = ui->zfDirectoryText->text().toStdString();

  if ( modelZFDirectory != guiZFDirectory ) {
    ui->zfDirectoryText->setText( QApplication::translate( context,
                                            modelZFDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );

    updateListing( &CMAQFileReaderModel::getZFDirectoryListing,
                   ui->zfDirectoryListingText );
  }

  const std::string modelWWINDDirectory = model.getWWINDDirectory();
  const std::string guiWWINDDirectory =
    ui->wwindDirectoryText->text().toStdString();

  if ( modelWWINDDirectory != guiWWINDDirectory ) {
    ui->wwindDirectoryText->setText( QApplication::translate( context,
                                            modelWWINDDirectory.c_str(), 0,
                                              QApplication::UnicodeUTF8 ) );

    updateListing( &CMAQFileReaderModel::getWWINDDirectoryListing,
                   ui->wwindDirectoryListingText );
  }

  QApplication::restoreOverrideCursor(); // Normal mouse cursor.
}



/******************************************************************************
PURPOSE: onSelectedDataVariable - Called when data variable is selected.
INPUTS:  QListWidgetItem* item  Selected data variable to read.
******************************************************************************/

void CMAQFileReaderDialog::onSelectedDataVariable( QListWidgetItem* item ) {

  if ( item ) {
    const QString itemText( item->text() );

    if ( ! itemText.isEmpty() ) {
      model.setDataVariable( itemText.toStdString() );
    }
  }
}



/******************************************************************************
PURPOSE: onReadVariable - Called when read button is clicked.
******************************************************************************/

void CMAQFileReaderDialog::onReadVariable() {
  int ok = 0;

  if ( AND3( ! model.getDataDirectory().empty(),
             ! model.getDataFile().empty(),
             ! model.getDataVariable().empty() ) ) {
    QApplication::setOverrideCursor( Qt::WaitCursor ); // Hourglass.
    model.readCMAQFile(); // SLOW.
    const std::string localSubsetFile = model.getOutputFile();

    if ( localSubsetFile.empty() ) { // Failed to create local subset file.
      const char* const failureMessageString =
        "I'm sorry, CMAQSubset program failed to create a local subset file.\n";
      const char* const name = "failureMessage";
      QMessageBox::warning( this, tr( name ), tr( failureMessageString ),
                            QMessageBox::Cancel );
    } else {
      ok = 1;
    }

    QApplication::restoreOverrideCursor(); // Normal mouse cursor.
 }

  done( ok ); // Close dialog.
}



/******************************************************************************
PURPOSE: onCancel - Called when cancel button is pressed.
******************************************************************************/

void CMAQFileReaderDialog::onCancel() {
  model.clearOutputFile();
  done( 0 );// Close dialog.
}



