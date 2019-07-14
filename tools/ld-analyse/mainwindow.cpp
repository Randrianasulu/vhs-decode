/************************************************************************

    mainwindow.cpp

    ld-analyse - TBC output analysis
    Copyright (C) 2018-2019 Simon Inns

    This file is part of ld-decode-tools.

    ld-analyse is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString inputFilenameParam, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the application's configuration
    configuration = new Configuration();

    // Add a status bar to show the state of the source video file
    ui->statusBar->addWidget(&sourceVideoStatus);
    ui->statusBar->addPermanentWidget(&frameLineStatus);
    sourceVideoStatus.setText(tr("No source video file loaded"));
    frameLineStatus.setText(tr(""));

    // Set the initial frame number
    currentFrameNumber = 1;
    isFileOpen = false;

    // Add an event filter to the frame viewer label to catch mouse events
    ui->frameViewerLabel->installEventFilter(ui->frameViewerLabel);
    connect(ui->frameViewerLabel, &FrameQLabel::mouseOverQFrame, this, &MainWindow::mouseOverQFrameSignalHandler);

    // Set up the oscilloscope dialogue
    oscilloscopeDialog = new OscilloscopeDialog(this);

    // Connect to the scan line changed signal from the oscilloscope dialogue
    connect(oscilloscopeDialog, &OscilloscopeDialog::scanLineChanged, this, &MainWindow::scanLineChangedSignalHandler);
    lastScopeLine = 1;

    // Set up the about dialogue
    aboutDialog = new AboutDialog(this);

    // Set up the VBI dialogue
    vbiDialog = new VbiDialog(this);

    // Set up the NTSC dialogue
    ntscDialog = new NtscDialog(this);

    // Set up the video metadata dialogue
    videoMetadataDialog = new VideoMetadataDialog(this);

    // Set up the dropout analysis dialogue
    dropoutAnalysisDialog = new DropoutAnalysisDialog(this);

    // Set up the VITS metrics dialogue
    vitsMetricsDialog = new VitsMetricsDialog(this);

    // Set up the SNR analysis dialogue
    snrAnalysisDialog = new SnrAnalysisDialog(this);

    // Set up the busy dialogue
    busyDialog = new BusyDialog(this);

    // Load the window geometry from the configuration
    restoreGeometry(configuration->getMainWindowGeometry());
    vbiDialog->restoreGeometry(configuration->getVbiDialogGeometry());
    ntscDialog->restoreGeometry(configuration->getNtscDialogGeometry());
    videoMetadataDialog->restoreGeometry(configuration->getVideoMetadataDialogGeometry());
    oscilloscopeDialog->restoreGeometry(configuration->getOscilloscopeDialogGeometry());
    dropoutAnalysisDialog->restoreGeometry(configuration->getDropoutAnalysisDialogGeometry());
    vitsMetricsDialog->restoreGeometry(configuration->getVitsMetricsDialogGeometry());
    snrAnalysisDialog->restoreGeometry(configuration->getSnrAnalysisDialogGeometry());

    updateGuiUnloaded();

    // Was a filename specified on the command line?
    if (!inputFilenameParam.isEmpty()) {
        loadTbcFile(inputFilenameParam);
    }
}

MainWindow::~MainWindow()
{
    // Save the window geometry to the configuration
    configuration->setMainWindowGeometry(saveGeometry());
    configuration->setVbiDialogGeometry(vbiDialog->saveGeometry());
    configuration->setNtscDialogGeometry(ntscDialog->saveGeometry());
    configuration->setVideoMetadataDialogGeometry(videoMetadataDialog->saveGeometry());
    configuration->setOscilloscopeDialogGeometry(oscilloscopeDialog->saveGeometry());
    configuration->setDropoutAnalysisDialogGeometry(dropoutAnalysisDialog->saveGeometry());
    configuration->setVitsMetricsDialogGeometry(vitsMetricsDialog->saveGeometry());
    configuration->setSnrAnalysisDialogGeometry(snrAnalysisDialog->saveGeometry());
    configuration->writeConfiguration();

    // Close the source video if open
    sourceVideo.close();
    delete ui;
}

// Method to update the GUI when a file is loaded
void MainWindow::updateGuiLoaded(void)
{
    // Get the video parameters
    LdDecodeMetaData::VideoParameters videoParameters = ldDecodeMetaData.getVideoParameters();

    // Enable the frame controls
    ui->frameNumberSpinBox->setEnabled(true);
    ui->previousPushButton->setEnabled(true);
    ui->nextPushButton->setEnabled(true);
    ui->frameHorizontalSlider->setEnabled(true);
    ui->combFilterRadioButton->setEnabled(true);
    ui->sourceRadioButton->setEnabled(true);

    // Enable the option check boxes
    ui->highlightDropOutsCheckBox->setEnabled(true);
    ui->showActiveVideoCheckBox->setEnabled(true);
    ui->reverseFieldOrderCheckBox->setEnabled(true);

    // Update the current frame number
    currentFrameNumber = 1;
    ui->frameNumberSpinBox->setMinimum(1);
    ui->frameNumberSpinBox->setMaximum(ldDecodeMetaData.getNumberOfFrames());
    currentFrameNumber = 1;
    ui->frameNumberSpinBox->setValue(currentFrameNumber);
    ui->frameHorizontalSlider->setMinimum(1);
    ui->frameHorizontalSlider->setMaximum(ldDecodeMetaData.getNumberOfFrames());
    currentFrameNumber = 1;
    ui->frameHorizontalSlider->setPageStep(ldDecodeMetaData.getNumberOfFrames() / 100);
    ui->frameHorizontalSlider->setValue(currentFrameNumber);

    // Enable the field information groupboxes
    ui->firstFieldGroupBox->setEnabled(true);
    ui->secondFieldGroupBox->setEnabled(true);

    // Enable menu options
    ui->actionLine_scope->setEnabled(true);
    ui->actionVBI->setEnabled(true);
    ui->actionNTSC->setEnabled(true);
    ui->actionVideo_metadata->setEnabled(true);
    ui->actionVITS_Metrics->setEnabled(true);
    ui->action1_1_Frame_size->setEnabled(true);
    ui->actionDropout_analysis->setEnabled(true);
    ui->actionSNR_analysis->setEnabled(true);
    ui->actionSave_frame_as_PNG->setEnabled(true);
    ui->actionSave_metadata_as_CSV->setEnabled(true);

    // Configure the comb-filter
    if (ldDecodeMetaData.getVideoParameters().isSourcePal) {
        palColour.updateConfiguration(videoParameters);
    } else {
        // Set the first active scan line
        qint32 firstActiveScanLine = 40;

        // Get the default configuration for the comb filter
        Comb::Configuration configuration = ntscColour.getConfiguration();

        // Set the comb filter configuration
        configuration.blackAndWhite = false;

        // Set the input buffer dimensions configuration
        configuration.fieldWidth = videoParameters.fieldWidth;
        configuration.fieldHeight = videoParameters.fieldHeight;

        // Set the active video range
        configuration.activeVideoStart = videoParameters.activeVideoStart;
        configuration.activeVideoEnd = videoParameters.activeVideoEnd;

        // Set the first frame scan line which contains active video
        configuration.firstVisibleFrameLine = firstActiveScanLine;

        // Set the IRE levels
        configuration.blackIre = videoParameters.black16bIre;
        configuration.whiteIre = videoParameters.white16bIre;

        // Set the filter mode
        configuration.use3D = false;
        configuration.showOpticalFlowMap = false;

        // Update the comb filter object's configuration
        ntscColour.setConfiguration(configuration);
    }

    // Update the status bar
    QString statusText;
    if (videoParameters.isSourcePal) statusText += "PAL";
    else statusText += "NTSC";
    statusText += " source loaded with ";
    statusText += QString::number(ldDecodeMetaData.getNumberOfFrames());
    statusText += " sequential frames available";
    sourceVideoStatus.setText(statusText);

    // Show the current frame
    showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());

    isFileOpen = true;
}

// Method to update the GUI when a file is unloaded
void MainWindow::updateGuiUnloaded(void)
{
    // Disable the frame controls
    ui->frameNumberSpinBox->setEnabled(false);
    ui->previousPushButton->setEnabled(false);
    ui->nextPushButton->setEnabled(false);
    ui->frameHorizontalSlider->setEnabled(false);
    ui->combFilterRadioButton->setEnabled(false);
    ui->sourceRadioButton->setEnabled(false);

    // Disable the option check boxes
    ui->highlightDropOutsCheckBox->setEnabled(false);
    ui->showActiveVideoCheckBox->setEnabled(false);
    ui->reverseFieldOrderCheckBox->setEnabled(false);
    ui->highlightDropOutsCheckBox->setChecked(false);
    ui->showActiveVideoCheckBox->setChecked(false);
    ui->reverseFieldOrderCheckBox->setChecked(false);

    // Update the current frame number
    currentFrameNumber = 1;
    ui->frameNumberSpinBox->setValue(currentFrameNumber);
    currentFrameNumber = 1;
    ui->frameHorizontalSlider->setValue(currentFrameNumber);
    currentFrameNumber = 1;

    // Allow the next and previous frame buttons to auto-repeat
    ui->previousPushButton->setAutoRepeat(true);
    ui->previousPushButton->setAutoRepeatDelay(500);
    ui->previousPushButton->setAutoRepeatInterval(10);
    ui->nextPushButton->setAutoRepeat(true);
    ui->nextPushButton->setAutoRepeatDelay(500);
    ui->nextPushButton->setAutoRepeatInterval(10);

    // Disable the field information groupboxes
    ui->firstFieldGroupBox->setEnabled(false);
    ui->secondFieldGroupBox->setEnabled(false);

    // Set the window title
    this->setWindowTitle(tr("ld-analyse"));

    // Set the status bar text
    sourceVideoStatus.setText(tr("No source video file loaded"));

    // Disable menu options
    ui->actionLine_scope->setEnabled(false);
    ui->actionVBI->setEnabled(false);
    ui->actionNTSC->setEnabled(false);
    ui->actionVideo_metadata->setEnabled(false);
    ui->actionVITS_Metrics->setEnabled(false);
    ui->action1_1_Frame_size->setEnabled(false);
    ui->actionDropout_analysis->setEnabled(false);
    ui->actionSNR_analysis->setEnabled(false);
    ui->actionSave_frame_as_PNG->setEnabled(false);
    ui->actionSave_metadata_as_CSV->setEnabled(false);

    // Hide the displayed frame
    hideFrame();

    // Hide graphs
    snrAnalysisDialog->hide();
    dropoutAnalysisDialog->hide();

    isFileOpen = false;
}

// Method to display a sequential frame
void MainWindow::showFrame(qint32 frameNumber, bool showActiveVideoArea, bool highlightDropOuts)
{
    // Get the required field numbers
    qint32 firstFieldNumber = ldDecodeMetaData.getFirstFieldNumber(frameNumber);
    qint32 secondFieldNumber = ldDecodeMetaData.getSecondFieldNumber(frameNumber);

    // Make sure we have a valid response from the frame determination
    if (firstFieldNumber == -1 || secondFieldNumber == -1) {
        QMessageBox messageBox;
        messageBox.warning(this, "Warning","Could not determine field numbers - check the debug!");
        messageBox.setFixedSize(500, 200);

        // Jump back one frame
        if (frameNumber != 1) {
            frameNumber--;

            firstFieldNumber = ldDecodeMetaData.getFirstFieldNumber(frameNumber);
            secondFieldNumber = ldDecodeMetaData.getSecondFieldNumber(frameNumber);
        }
        qDebug() << "MainWindow::showFrame(): Jumping back one frame due to error";
    }

    qDebug() << "MainWindow::showFrame(): Frame number" << frameNumber << "has a first-field of" << firstFieldNumber <<
                "and a second field of" << secondFieldNumber;

    // Get a QImage for the frame
    QImage frameImage = generateQImage(firstFieldNumber, secondFieldNumber);

    // Get the field metadata
    LdDecodeMetaData::Field firstField = ldDecodeMetaData.getField(firstFieldNumber);
    LdDecodeMetaData::Field secondField = ldDecodeMetaData.getField(secondFieldNumber);

    // Show the field numbers
    ui->firstFieldLabel->setText(QString::number(firstFieldNumber));
    ui->secondFieldLabel->setText(QString::number(secondFieldNumber));

    // Get the video parameters
    LdDecodeMetaData::VideoParameters videoParameters = ldDecodeMetaData.getVideoParameters();

    // Show the active video extent?
    if (showActiveVideoArea) {
        // Create a painter object
        QPainter imagePainter;

        imagePainter.begin(&frameImage);
        imagePainter.setPen(Qt::cyan);

        // Determine the first and last active scan line based on the source format
        qint32 firstActiveScanLine;
        qint32 lastActiveScanLine;
        if (videoParameters.isSourcePal) {
            firstActiveScanLine = 44;
            lastActiveScanLine = 620;
        } else {
            firstActiveScanLine = 40;
            lastActiveScanLine = 525;
        }

        // Outline the active video area
        imagePainter.drawLine(videoParameters.activeVideoStart, firstActiveScanLine, videoParameters.activeVideoStart, lastActiveScanLine);
        imagePainter.drawLine(videoParameters.activeVideoEnd, firstActiveScanLine, videoParameters.activeVideoEnd, lastActiveScanLine);
        imagePainter.drawLine(videoParameters.activeVideoStart, firstActiveScanLine, videoParameters.activeVideoEnd, firstActiveScanLine);
        imagePainter.drawLine(videoParameters.activeVideoStart, lastActiveScanLine, videoParameters.activeVideoEnd, lastActiveScanLine);

        // Outline the VP415 Domesday player active video area
        qreal vp415FirstActiveScanLine = firstActiveScanLine + (((videoParameters.fieldHeight * 2) / 100) * 1.0);
        qreal vp415LastActiveScanLine = lastActiveScanLine - (((videoParameters.fieldHeight * 2) / 100) * 1.0);
        qreal vp415VideoStart = videoParameters.activeVideoStart + ((videoParameters.fieldWidth / 100) * 1.0);
        qreal vp415VideoEnd = videoParameters.activeVideoEnd - ((videoParameters.fieldWidth / 100) * 1.0);
        imagePainter.drawLine(static_cast<qint32>(vp415VideoStart), static_cast<qint32>(vp415FirstActiveScanLine),
                              static_cast<qint32>(vp415VideoStart), static_cast<qint32>(vp415LastActiveScanLine));
        imagePainter.drawLine(static_cast<qint32>(vp415VideoEnd), static_cast<qint32>(vp415FirstActiveScanLine),
                              static_cast<qint32>(vp415VideoEnd), static_cast<qint32>(vp415LastActiveScanLine));

        imagePainter.drawLine(static_cast<qint32>(vp415VideoStart), static_cast<qint32>(vp415FirstActiveScanLine),
                              static_cast<qint32>(vp415VideoEnd), static_cast<qint32>(vp415FirstActiveScanLine));
        imagePainter.drawLine(static_cast<qint32>(vp415VideoStart), static_cast<qint32>(vp415LastActiveScanLine),
                              static_cast<qint32>(vp415VideoEnd), static_cast<qint32>(vp415LastActiveScanLine));

        // End the painter object
        imagePainter.end();
    }

    if (highlightDropOuts) {
        // Create a painter object
        QPainter imagePainter;
        imagePainter.begin(&frameImage);

        // Draw the drop out data for the first field
        imagePainter.setPen(Qt::red);
        for (qint32 dropOutIndex = 0; dropOutIndex < firstField.dropOuts.startx.size(); dropOutIndex++) {
            qint32 startx = firstField.dropOuts.startx[dropOutIndex];
            qint32 endx = firstField.dropOuts.endx[dropOutIndex];
            qint32 fieldLine = firstField.dropOuts.fieldLine[dropOutIndex];

            imagePainter.drawLine(startx, ((fieldLine - 1) * 2), endx, ((fieldLine - 1) * 2));
        }

        // Draw the drop out data for the second field
        imagePainter.setPen(Qt::blue);
        for (qint32 dropOutIndex = 0; dropOutIndex < secondField.dropOuts.startx.size(); dropOutIndex++) {
            qint32 startx = secondField.dropOuts.startx[dropOutIndex];
            qint32 endx = secondField.dropOuts.endx[dropOutIndex];
            qint32 fieldLine = secondField.dropOuts.fieldLine[dropOutIndex];

            imagePainter.drawLine(startx, ((fieldLine - 1) * 2) + 1, endx, ((fieldLine - 1) * 2) + 1);
        }

        // End the painter object
        imagePainter.end();
    }

    // Add the first field VBI data to the dialogue
    if (firstField.vbi.inUse) {
        ui->firstField0VbiLabel->setText("0x" + QString::number(firstField.vbi.vbiData[0], 16));
        ui->firstField1VbiLabel->setText("0x" + QString::number(firstField.vbi.vbiData[1], 16));
        ui->firstField2VbiLabel->setText("0x" + QString::number(firstField.vbi.vbiData[2], 16));
    } else {
        ui->firstField0VbiLabel->setText("No metadata");
        ui->firstField1VbiLabel->setText("No metadata");
        ui->firstField2VbiLabel->setText("No metadata");
    }

    // Add the first field sync confidence field to the dialogue
    ui->firstFieldSyncConfLabel->setText(QString::number(firstField.syncConf) + "%");

    // Add the first field drop out data to the dialogue
    if (firstField.dropOuts.startx.size() > 0) {
        ui->firstFieldDropoutsLabel->setText(QString::number(firstField.dropOuts.startx.size()));

        // Calculate the total length of the dropouts
        qint32 doLength = 0;
        for (qint32 i = 0; i < firstField.dropOuts.startx.size(); i++) {
            doLength += firstField.dropOuts.endx[i] - firstField.dropOuts.startx[i];
        }
        ui->firstFieldDoLengthLabel->setText(QString::number(doLength));
    } else {
        ui->firstFieldDropoutsLabel->setText("0");
        ui->firstFieldDoLengthLabel->setText("0");
    }

    // Add the second field VBI data to the dialogue
    if (secondField.vbi.inUse) {
        ui->secondField0VbiLabel->setText("0x" + QString::number(secondField.vbi.vbiData[0], 16));
        ui->secondField1VbiLabel->setText("0x" + QString::number(secondField.vbi.vbiData[1], 16));
        ui->secondField2VbiLabel->setText("0x" + QString::number(secondField.vbi.vbiData[2], 16));
    } else {
        ui->secondField0VbiLabel->setText("No metadata");
        ui->secondField1VbiLabel->setText("No metadata");
        ui->secondField2VbiLabel->setText("No metadata");
    }

    // Add the second field sync confidence field to the dialogue
    ui->secondFieldSyncConfLabel->setText(QString::number(secondField.syncConf) + "%");

    // Add the second field drop out data to the dialogue
    if (secondField.dropOuts.startx.size() > 0) {
        ui->secondFieldDropoutsLabel->setText(QString::number(secondField.dropOuts.startx.size()));

        // Calculate the total length of the dropouts
        qint32 doLength = 0;
        for (qint32 i = 0; i < secondField.dropOuts.startx.size(); i++) {
            doLength += secondField.dropOuts.endx[i] - secondField.dropOuts.startx[i];
        }
        ui->secondFieldDoLengthLabel->setText(QString::number(doLength));
    } else {
        ui->secondFieldDropoutsLabel->setText("0");
        ui->secondFieldDoLengthLabel->setText("0");
    }

    // Update the VBI dialogue
    vbiDialog->updateVbi(firstField, secondField);

    // Update the NTSC dialogue
    ntscDialog->updateNtsc(firstField, secondField);

    // Update the metadata dialogue
    videoMetadataDialog->updateMetaData(videoParameters);

    // Update the VITS metrics dialogue
    vitsMetricsDialog->updateVitsMetrics(firstField, secondField);

    // Add the QImage to the QLabel in the dialogue
    ui->frameViewerLabel->clear();
    ui->frameViewerLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ui->frameViewerLabel->setAlignment(Qt::AlignCenter);
    ui->frameViewerLabel->setMinimumSize(frameImage.width(), frameImage.height());
    ui->frameViewerLabel->setScaledContents(false);
    ui->frameViewerLabel->setPixmap(QPixmap::fromImage(frameImage));

    // If the scope window is open, update it too (using the last scope line selected by the user)
    if (oscilloscopeDialog->isVisible()) {
        // Show the oscilloscope dialogue for the selected scan-line
        updateOscilloscopeDialogue(currentFrameNumber, lastScopeLine);
    }
}

// Method to create a QImage for a source video frame
QImage MainWindow::generateQImage(qint32 firstFieldNumber, qint32 secondFieldNumber)
{
    // Generate the QImage for the frame

    // Get the metadata for the video parameters
    LdDecodeMetaData::VideoParameters videoParameters = ldDecodeMetaData.getVideoParameters();

    // Get the raw data for the fields (without pre-caching)
    QByteArray firstFieldData = sourceVideo.getVideoField(firstFieldNumber, true);
    QByteArray secondFieldData = sourceVideo.getVideoField(secondFieldNumber, true);

    // Calculate the frame height
    qint32 frameHeight = (videoParameters.fieldHeight * 2) - 1;

    qDebug() << "MainWindow::generateQImage(): Generating a QImage with first field =" << firstFieldNumber <<
                "and second field =" << secondFieldNumber << "(" << videoParameters.fieldWidth << "x" <<
                frameHeight << ")";

    // Create a QImage
    QImage frameImage = QImage(videoParameters.fieldWidth, frameHeight, QImage::Format_RGB888);

    // Define the data buffers
    QByteArray firstLineData;
    QByteArray secondLineData;

    if (ui->sourceRadioButton->isChecked()) {
        // Copy the raw 16-bit grayscale data into the RGB888 QImage
        for (qint32 y = 0; y < frameHeight; y++) {
            // Extract the current scan line data from the frame
            qint32 startPointer = (y / 2) * videoParameters.fieldWidth * 2;
            qint32 length = videoParameters.fieldWidth * 2;

            firstLineData = firstFieldData.mid(startPointer, length);
            secondLineData = secondFieldData.mid(startPointer, length);

            for (qint32 x = 0; x < videoParameters.fieldWidth; x++) {
                // Take just the MSB of the input data
                qint32 dp = x * 2;
                uchar pixelValue;
                if (y % 2) {
                    pixelValue = static_cast<uchar>(secondLineData[dp + 1]);
                } else {
                    pixelValue = static_cast<uchar>(firstLineData[dp + 1]);
                }

                qint32 xpp = x * 3;
                *(frameImage.scanLine(y) + xpp + 0) = static_cast<uchar>(pixelValue); // R
                *(frameImage.scanLine(y) + xpp + 1) = static_cast<uchar>(pixelValue); // G
                *(frameImage.scanLine(y) + xpp + 2) = static_cast<uchar>(pixelValue); // B
            }
        }
    } else {
        // Set the first and last active scan line (for PAL)
        qint32 firstActiveScanLine = 44;
        qint32 lastActiveScanLine = 620;
        QByteArray outputData;

        // Perform a PAL 2D comb filter on the current frame
        if (videoParameters.isSourcePal) {
            // PAL source

            // Calculate the saturation level from the burst median IRE
            // Note: This code works as a temporary MTF compensator whilst ld-decode gets
            // real MTF compensation added to it.
            qreal tSaturation = 125.0 + ((100.0 / 20.0) * (20.0 - ldDecodeMetaData.getField(firstFieldNumber).medianBurstIRE));

            // Perform the PALcolour filtering (output is RGB 16-16-16)
            outputData = palColour.performDecode(firstFieldData, secondFieldData,
                                                  100, static_cast<qint32>(tSaturation), false);
        } else {
            // NTSC source

            // Set the first and last active scan line
            firstActiveScanLine = 40;
            lastActiveScanLine = 525;

            outputData = ntscColour.process(firstFieldData, secondFieldData,
                                                            ldDecodeMetaData.getField(firstFieldNumber).medianBurstIRE,
                                                            ldDecodeMetaData.getField(firstFieldNumber).fieldPhaseID,
                                                            ldDecodeMetaData.getField(secondFieldNumber).fieldPhaseID);
        }

        // Fill the QImage with black
        frameImage.fill(Qt::black);

        // Copy the RGB16-16-16 data into the RGB888 QImage
        for (qint32 y = firstActiveScanLine; y < lastActiveScanLine; y++) {
            // Extract the current scan line data from the frame
            qint32 startPointer = y * videoParameters.fieldWidth * 6;
            qint32 length = videoParameters.fieldWidth * 6;

            QByteArray rgbData = outputData.mid(startPointer, length);

            for (qint32 x = videoParameters.activeVideoStart; x < videoParameters.activeVideoEnd; x++) {
                // Take just the MSB of the input data
                qint32 dp = x * 6;

                uchar pixelValueR = static_cast<uchar>(rgbData[dp + 1]);
                uchar pixelValueG = static_cast<uchar>(rgbData[dp + 3]);
                uchar pixelValueB = static_cast<uchar>(rgbData[dp + 5]);

                qint32 xpp = x * 3;
                *(frameImage.scanLine(y) + xpp + 0) = static_cast<uchar>(pixelValueR); // R
                *(frameImage.scanLine(y) + xpp + 1) = static_cast<uchar>(pixelValueG); // G
                *(frameImage.scanLine(y) + xpp + 2) = static_cast<uchar>(pixelValueB); // B
            }
        }
    }

    return frameImage;
}

// Method to hide the current frame
void MainWindow::hideFrame(void)
{
    ui->frameViewerLabel->clear();
}

void MainWindow::on_actionExit_triggered()
{
    qDebug() << "MainWindow::on_actionExit_triggered(): Called";
        // Quit the application
    qApp->quit();
}

// Load a TBC file based on the passed file name
void MainWindow::loadTbcFile(QString inputFileName)
{
    // Update the GUI
    updateGuiUnloaded();

    // Close current source video (if open)
    sourceVideo.close();

    // Set the busy message and centre the dialog in the parent window
    busyDialog->setMessaage("Processing...");
    busyDialog->move(this->geometry().center() - busyDialog->rect().center());

    // Disable the main window during loading
    this->setEnabled(false);
    busyDialog->setEnabled(true);

    QFutureWatcher<void> watcher;
    connect(&watcher, SIGNAL(finished()), this, SLOT(blackgroundLoadComplete()));

    QFuture <void> future = QtConcurrent::run(this, &MainWindow::backgroundLoad, inputFileName);
    watcher.setFuture(future);

    busyDialog->exec();
}

// Post-background loading operations
void MainWindow::blackgroundLoadComplete()
{
    qDebug() << "MainWindow::blackgroundLoadComplete(): Background loading is complete";

    // Hide the busy dialogue and enable the main window
    busyDialog->hide();
    this->setEnabled(true);

    // Update the GUI based on the result
    if (isFileOpen) {
        updateGuiLoaded();
    } else {
        updateGuiUnloaded();

        // Show an error to the user
        QMessageBox messageBox;
        messageBox.warning(this, "Error", lastLoadError);
        messageBox.setFixedSize(500, 200);
    }
}

// Load the TBC and JSON metadata (called by QFuture in loadTbcFile method)
void MainWindow::backgroundLoad(QString inputFileName)
{
    qInfo() << "Opening TBC filename =" << inputFileName;
    isFileOpen = false;

    // Open the TBC metadata file
    qDebug() << "MainWindow::backgroundLoad(): Processing JSON metadata...";
    busyDialog->setMessaage("Processing JSON metadata...");
    if (!ldDecodeMetaData.read(inputFileName + ".json")) {
        // Open failed
        qWarning() << "Open TBC JSON metadata failed for filename" << inputFileName;
        currentInputFileName.clear();

        // Show an error to the user
        lastLoadError = "Could not open TBC JSON metadata file for the TBC input file!";
    } else {
        // Get the video parameters from the metadata
        LdDecodeMetaData::VideoParameters videoParameters = ldDecodeMetaData.getVideoParameters();

        // Open the new source video
        qDebug() << "MainWindow::backgroundLoad(): Loading TBC file...";
        busyDialog->setMessaage("Loading TBC file...");
        if (!sourceVideo.open(inputFileName, videoParameters.fieldWidth * videoParameters.fieldHeight)) {
            // Open failed
            qWarning() << "Open TBC file failed for filename" << inputFileName;
            currentInputFileName.clear();

            // Show an error to the user
            lastLoadError = "Could not open TBC data file!";
        } else {
            // Both the video and metadata files are now open

            // Update the configuration for the source directory
            QFileInfo inFileInfo(inputFileName);
            configuration->setSourceDirectory(inFileInfo.absolutePath());
            qDebug() << "MainWindow::backgroundLoad(): Setting source directory to:" << inFileInfo.absolutePath();
            configuration->writeConfiguration();

            qDebug() << "MainWindow::backgroundLoad(): Generating DO and SNR graphs...";
            busyDialog->setMessaage("Generating drop-out analysis graph...");
            dropoutAnalysisDialog->updateChart(&ldDecodeMetaData);
            busyDialog->setMessaage("Generating SNR analysis graph...");
            snrAnalysisDialog->updateChart(&ldDecodeMetaData);

            busyDialog->setMessaage("Loading completed");
            isFileOpen = true;

            // Set the window title
            this->setWindowTitle(tr("ld-analyse - ") + inputFileName);

            // Set the current file name
            currentInputFileName = inFileInfo.fileName();
            qDebug() << "MainWindow::backgroundLoad(): Set current file name to to:" << currentInputFileName;
        }
    }
}

// Load a TBC file based on the file selection from the GUI
void MainWindow::on_actionOpen_TBC_file_triggered()
{
    qDebug() << "MainWindow::on_actionOpen_TBC_file_triggered(): Called";

    QString inputFileName = QFileDialog::getOpenFileName(this,
                tr("Open TBC file"),
                configuration->getSourceDirectory()+tr("/ldsample.tbc"),
                tr("TBC output (*.tbc);;All Files (*)"));

    // Was a filename specified?
    if (!inputFileName.isEmpty() && !inputFileName.isNull()) {
        loadTbcFile(inputFileName);
    }
}

// Previous frame button has been clicked
void MainWindow::on_previousPushButton_clicked()
{
    currentFrameNumber--;
    if (currentFrameNumber < 1) {
        currentFrameNumber = 1;
    } else {
        ui->frameNumberSpinBox->setValue(currentFrameNumber);
        ui->frameHorizontalSlider->setValue(currentFrameNumber);
        showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
    }
}

// Next frame button has been clicked
void MainWindow::on_nextPushButton_clicked()
{
    currentFrameNumber++;
    if (currentFrameNumber > ldDecodeMetaData.getNumberOfFrames()) {
        currentFrameNumber = ldDecodeMetaData.getNumberOfFrames();
    } else {
        ui->frameNumberSpinBox->setValue(currentFrameNumber);
        ui->frameHorizontalSlider->setValue(currentFrameNumber);
        showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
    }
}

// Frame number spin box editing has finished
void MainWindow::on_frameNumberSpinBox_editingFinished()
{
    if (ui->frameNumberSpinBox->value() != currentFrameNumber) {
        if (ui->frameNumberSpinBox->value() < 1) ui->frameNumberSpinBox->setValue(1);
        if (ui->frameNumberSpinBox->value() > sourceVideo.getNumberOfAvailableFields()) ui->frameNumberSpinBox->setValue(ldDecodeMetaData.getNumberOfFrames());
        currentFrameNumber = ui->frameNumberSpinBox->value();
        ui->frameHorizontalSlider->setValue(currentFrameNumber);
        showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
    }
}

// Frame slider value has changed
void MainWindow::on_frameHorizontalSlider_valueChanged(int value)
{
    (void)value;

    if (!isFileOpen) return;
    qDebug() << "MainWindow::on_frameHorizontalSlider_valueChanged(): Called";

    currentFrameNumber = ui->frameHorizontalSlider->value();

    // If the spinbox is enabled, we can update the current frame number
    // otherwisew we just ignore this
    if (ui->frameNumberSpinBox->isEnabled()) {
        ui->frameNumberSpinBox->setValue(currentFrameNumber);
        showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
    }
}


void MainWindow::on_showActiveVideoCheckBox_clicked()
{
    showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
}

void MainWindow::on_highlightDropOutsCheckBox_clicked()
{
    showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
}

void MainWindow::on_actionLine_scope_triggered()
{
    if (isFileOpen) {
        // Show the oscilloscope dialogue for the selected scan-line
        updateOscilloscopeDialogue(currentFrameNumber, lastScopeLine);
        oscilloscopeDialog->show();
    }
}

void MainWindow::on_actionAbout_ld_analyse_triggered()
{
    aboutDialog->show();
}

// Display video metadata dialogue triggered
void MainWindow::on_actionVideo_metadata_triggered()
{
    videoMetadataDialog->show();
}

void MainWindow::on_actionVBI_triggered()
{
    // Show the VBI dialogue
    vbiDialog->show();
}

void MainWindow::on_actionNTSC_triggered()
{
    // Show the NTSC dialogue
    ntscDialog->show();
}

void MainWindow::on_actionDropout_analysis_triggered()
{
    // Show the dropout analysis dialogue
    dropoutAnalysisDialog->show();
}

void MainWindow::on_actionVITS_Metrics_triggered()
{
    // Show the VITS metrics dialogue
    vitsMetricsDialog->show();
}

void MainWindow::on_actionSNR_analysis_triggered()
{
    // Show the SNR analysis dialogue
    snrAnalysisDialog->show();
}

// Adjust the window to show the frame at 1:1 zoom
void MainWindow::on_action1_1_Frame_size_triggered()
{
    this->resize(minimumSizeHint());
    this->adjustSize();
    this->resize(sizeHint());
}

// Display source frame radio button clicked
void MainWindow::on_sourceRadioButton_clicked()
{
    // Show the current frame
    showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
}

// Display comb-filtered frame
void MainWindow::on_combFilterRadioButton_clicked()
{
    // Show the current frame
    showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
}

// Save current frame as PNG
void MainWindow::on_actionSave_frame_as_PNG_triggered()
{
    qDebug() << "MainWindow::on_actionSave_frame_as_PNG_triggered(): Called";

    // Create a suggestion for the filename
    QString filenameSuggestion = configuration->getPngDirectory();
    if (ldDecodeMetaData.getVideoParameters().isSourcePal) filenameSuggestion += tr("/frame_pal_");
    else filenameSuggestion += tr("/frame_ntsc_");
    if (ui->sourceRadioButton->isChecked()) filenameSuggestion += tr("source_");
    else filenameSuggestion += tr("comb_");
    filenameSuggestion += QString::number(currentFrameNumber) + tr(".png");

    QString pngFilename = QFileDialog::getSaveFileName(this,
                tr("Save PNG file"),
                filenameSuggestion,
                tr("PNG image (*.png);;All Files (*)"));

    // Was a filename specified?
    if (!pngFilename.isEmpty() && !pngFilename.isNull()) {
        // Save the current frame as a PNG
        qDebug() << "MainWindow::on_actionSave_frame_as_PNG_triggered(): Saving current frame as" << pngFilename;

        // Get the required field numbers
        qint32 firstFieldNumber = ldDecodeMetaData.getFirstFieldNumber(currentFrameNumber);
        qint32 secondFieldNumber = ldDecodeMetaData.getSecondFieldNumber(currentFrameNumber);

        // Generate the current frame and save it
        if (!generateQImage(firstFieldNumber, secondFieldNumber).save(pngFilename)) {
            qDebug() << "MainWindow::on_actionSave_frame_as_PNG_triggered(): Failed to save file as" << pngFilename;

            QMessageBox messageBox;
            messageBox.warning(this, "Warning","Could not save a PNG using the specified filename!");
            messageBox.setFixedSize(500, 200);
        }

        // Update the configuration for the PNG directory
        QFileInfo pngFileInfo(pngFilename);
        configuration->setPngDirectory(pngFileInfo.absolutePath());
        qDebug() << "MainWindow::on_actionSave_frame_as_PNG_triggered(): Setting PNG directory to:" << pngFileInfo.absolutePath();
        configuration->writeConfiguration();
    }
}

// Save the VITS metadata as a CSV file
void MainWindow::on_actionSave_metadata_as_CSV_triggered()
{
    qDebug() << "MainWindow::on_actionSave_metadata_as_CSV_triggered(): Called";

    // Create a suggestion for the filename
    QString filenameSuggestion = configuration->getCsvDirectory() + tr("/");
    filenameSuggestion += currentInputFileName + tr(".csv");

    QString csvFilename = QFileDialog::getSaveFileName(this,
                tr("Save CSV file"),
                filenameSuggestion,
                tr("CSV file (*.csv);;All Files (*)"));

    // Was a filename specified?
    if (!csvFilename.isEmpty() && !csvFilename.isNull()) {
        // Save the metadata as CSV
        qDebug() << "MainWindow::on_actionSave_metadata_as_CSV_triggered(): Saving VITS metadata as" << csvFilename;

        ldDecodeMetaData.writeVitsCsv(csvFilename);

        // Update the configuration for the CSV directory
        QFileInfo csvFileInfo(csvFilename);
        configuration->setCsvDirectory(csvFileInfo.absolutePath());
        qDebug() << "MainWindow::on_actionSave_metadata_as_CSV_triggered(): Setting CSV directory to:" << csvFileInfo.absolutePath();
        configuration->writeConfiguration();
    }
}

void MainWindow::on_reverseFieldOrderCheckBox_stateChanged(int arg1)
{
    (void)arg1;

    if (ui->reverseFieldOrderCheckBox->isChecked()) ldDecodeMetaData.setIsFirstFieldFirst(false);
        else ldDecodeMetaData.setIsFirstFieldFirst(true);

    // If the TBC field order is changed, the number of available frames can change, so we need to update the GUI
    updateGuiLoaded();

    // Show the current frame
    showFrame(currentFrameNumber, ui->showActiveVideoCheckBox->isChecked(), ui->highlightDropOutsCheckBox->isChecked());
}

// Method to update the line oscilloscope based on the frame number and scan line
void MainWindow::updateOscilloscopeDialogue(qint32 frameNumber, qint32 scanLine)
{
    // Determine the first and second fields for the frame number
    qint32 firstFieldNumber = ldDecodeMetaData.getFirstFieldNumber(frameNumber);
    qint32 secondFieldNumber = ldDecodeMetaData.getSecondFieldNumber(frameNumber);

    // Update the oscilloscope dialogue
    oscilloscopeDialog->showTraceImage(sourceVideo.getVideoField(firstFieldNumber, true),
                                       sourceVideo.getVideoField(secondFieldNumber, true),
                                       &ldDecodeMetaData, scanLine, firstFieldNumber, secondFieldNumber);
}

void MainWindow::scanLineChangedSignalHandler(qint32 scanLine)
{
    qDebug() << "MainWindow::scanLineChangedSignalHandler(): Called with scanLine =" << scanLine;

    if (isFileOpen) {
        // Show the oscilloscope dialogue for the selected scan-line
        updateOscilloscopeDialogue(currentFrameNumber, scanLine);
        oscilloscopeDialog->show();

        // Remember the last line rendered
        lastScopeLine = scanLine;
    }
}

// Mouse press event handler
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!isFileOpen) return;

    // Get the mouse position relative to our scene
    QPoint origin = ui->frameViewerLabel->mapFromGlobal(QCursor::pos());

    // Get the metadata for the fields
    LdDecodeMetaData::VideoParameters videoParameters = ldDecodeMetaData.getVideoParameters();

    // Calculate the frame height
    qint32 frameHeight = (videoParameters.fieldHeight * 2) - 1;

    // Check that the mouse click is within bounds of the current picture
    qreal offset = ((static_cast<qreal>(ui->frameViewerLabel->height()) - static_cast<qreal>(ui->frameViewerLabel->pixmap()->height())) / 2.0);

    qint32 oX = origin.x();
    qint32 oY = origin.y();

    if (oX + 1 >= 0 &&
            oY >= 0 &&
            oX + 1 <= ui->frameViewerLabel->width() &&
            oY <= ui->frameViewerLabel->height()) {

        qreal unscaledYR = (static_cast<qreal>(frameHeight) / static_cast<qreal>(ui->frameViewerLabel->pixmap()->height())) * static_cast<qreal>(oY - offset);
        qint32 unscaledY = static_cast<qint32>(unscaledYR) + 1;
        if (unscaledY > frameHeight) unscaledY = frameHeight;
        if (unscaledY < 1) unscaledY = 1;

        // Show the oscilloscope dialogue for the selected scan-line
        updateOscilloscopeDialogue(currentFrameNumber, unscaledY);
        oscilloscopeDialog->show();

        // Remember the last line rendered
        lastScopeLine = origin.y();

        event->accept();
    }
}

// Method to handle the mouse over event from the frame viewer label
// (This updates the current line number in the status bar)
void MainWindow::mouseOverQFrameSignalHandler(QMouseEvent *event)
{
    if (!isFileOpen) return;

    // Get the mouse position relative to our scene
    QPoint origin = ui->frameViewerLabel->mapFromGlobal(QCursor::pos());

    // Get the metadata for the fields
    LdDecodeMetaData::VideoParameters videoParameters = ldDecodeMetaData.getVideoParameters();

    // Calculate the frame height
    qint32 frameHeight = (videoParameters.fieldHeight * 2) - 1;

    // Check that the mouse click is within bounds of the current picture
    qreal offset = ((static_cast<qreal>(ui->frameViewerLabel->height()) - static_cast<qreal>(ui->frameViewerLabel->pixmap()->height())) / 2.0);

    qint32 oX = origin.x();
    qint32 oY = origin.y();

    if (oX + 1 >= 0 &&
            oY >= 0 &&
            oX + 1 <= ui->frameViewerLabel->width() &&
            oY <= ui->frameViewerLabel->height()) {

        qreal unscaledYR = (static_cast<qreal>(frameHeight) / static_cast<qreal>(ui->frameViewerLabel->pixmap()->height())) * static_cast<qreal>(oY - offset);
        qint32 unscaledY = static_cast<qint32>(unscaledYR) + 1;
        if (unscaledY > frameHeight) unscaledY = frameHeight;
        if (unscaledY < 1) unscaledY = 1;

        frameLineStatus.setText(QString::number(unscaledY));
        event->accept();
    }
}







