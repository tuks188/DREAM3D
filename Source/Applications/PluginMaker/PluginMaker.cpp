/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * Copyright (c) 2012 Joseph B. Kleingers (Student Research Assistant)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, Joseph B. Kleingers,
 * the US Air Force, BlueQuartz Software nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "PluginMaker.h"
#include <iostream>




#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtGui/QWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QProgressBar>
#include <QtGui/QMessageBox>

#include "HelpWidget.h"
#include "DREAM3D/License/PluginMakerLicenseFiles.h"
#include "QtSupport/ApplicationAboutBoxDialog.h"
#include "PMDirGenerator.h"
#include "PMFileGenerator.h"
#include "PMFilterGenerator.h"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PluginMaker::PluginMaker(QWidget* parent) :
 QMainWindow(parent)
{
  setupUi(this);

  setupGui();


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PluginMaker::setupGui()
{

  QString pathTemplate;

  nameLabel->setToolTip("Plugin Name");
  m_PluginName->setToolTip("Enter Plugin Name Here");
  outputFileNameLabel->setToolTip("Output Directory");
  m_OutputDir->setToolTip("Enter Output Directory Here");
  selectButton->setToolTip("Select Directory");
  generateButton->setToolTip("Generate File Structure");
  aboutButton->setToolTip("About PluginMakerİ");

  QTreeWidgetItem* F_main = new QTreeWidgetItem(treeWidget);
  F_main->setText(0, "Unknown Plugin Name");
  {
     pathTemplate = "@PluginName@";
     QString resourceTemplate("");
     PMDirGenerator* gen = new PMDirGenerator(m_OutputDir->text(),
                                                     pathTemplate,
                                                     QString(""),
                                                     resourceTemplate,
                                                     F_main,
                                                     this);
     gen->setDisplaySuffix("");
     gen->setDoesGenerateOutput(false);
     gen->setNameChangeable(true);
     m_GenObjects.push_back(gen);
     connect(m_PluginName, SIGNAL(textChanged(const QString &)),
             gen, SLOT(pluginNameChanged(const QString &)));

   }


  QTreeWidgetItem* F_code = new QTreeWidgetItem(F_main);
  F_code->setText(0, tr("Code"));


  QTreeWidgetItem* F_doc = new QTreeWidgetItem(F_main);
  F_doc->setText(0, tr("Documentation"));

  QTreeWidgetItem* cmake = new QTreeWidgetItem(F_main);
  cmake->setText(0, tr("CMakeLists.txt"));
  {
    pathTemplate = "@PluginName@";
    QString resourceTemplate(":/Template/CMakeLists.txt.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString("CMakeLists.txt"),
                                                    resourceTemplate,
                                                    cmake,
                                                    this);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
    gen->setDoesGenerateOutput(true);
    m_GenObjects.push_back(gen);
  }




  QTreeWidgetItem* F_name = new QTreeWidgetItem(F_code);
  F_name->setText(0, "Unknown Plugin Name");
  {
     pathTemplate = "@PluginName@/Code/@PluginName@Filters/";
     QString resourceTemplate("");
     PMDirGenerator* gen = new PMDirGenerator(m_OutputDir->text(),
                                                     pathTemplate,
                                                     QString(""),
                                                     resourceTemplate,
                                                     F_name,
                                                     this);
     gen->setDisplaySuffix("Filters");
     gen->setDoesGenerateOutput(false);
     gen->setNameChangeable(true);
     m_GenObjects.push_back(gen);
     connect(m_PluginName, SIGNAL(textChanged(const QString &)),
             gen, SLOT(pluginNameChanged(const QString &)));

   }



  QTreeWidgetItem* plugincpp = new QTreeWidgetItem(F_code);
  plugincpp->setText(0, "Unknown Plugin Name");
  {
    pathTemplate = "@PluginName@/Code/";
    QString resourceTemplate(":/Template/Code/Plugin.cpp.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString(""),
                                                    resourceTemplate,
                                                    plugincpp,
                                                    this);
    gen->setDisplaySuffix("Plugin.cpp");
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(true);
    m_GenObjects.push_back(gen);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
  }

  QTreeWidgetItem* pluginh = new QTreeWidgetItem(F_code);
  pluginh->setText(0, "Unknown Plugin Name");
  {
    pathTemplate = "@PluginName@/Code/";
    QString resourceTemplate(":/Template/Code/Plugin.h.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString(""),
                                                    resourceTemplate,
                                                    pluginh,
                                                    this);
    gen->setDisplaySuffix("Plugin.h");
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(true);
    m_GenObjects.push_back(gen);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
  }




  QTreeWidgetItem* filtercpp = new QTreeWidgetItem(F_name);
  filtercpp->setText(0, "Unknown Plugin Name");
  {
    pathTemplate = "@PluginName@/Code/@PluginName@Filters/";
    QString resourceTemplate(":/Template/Code/Filter/Filter.cpp.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString(""),
                                                    resourceTemplate,
                                                    filtercpp,
                                                    this);
    gen->setDisplaySuffix("Filter.cpp");
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(true);
    m_FilterClasses.push_back(gen);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
  }

  QTreeWidgetItem* filterh = new QTreeWidgetItem(F_name);
  filterh->setText(0, "Unknown Plugin Name");
  {
    pathTemplate = "@PluginName@/Code/@PluginName@Filters/";
    QString resourceTemplate(":/Template/Code/Filter/Filter.h.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString(""),
                                                    resourceTemplate,
                                                    filterh,
                                                    this);
    gen->setDisplaySuffix("Filter.h");
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(true);
    m_FilterClasses.push_back(gen);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
  }



  /* This simulates the user clicking on the "Add Filter" button */
#if 0
  QTreeWidgetItem* filt2cpp = new QTreeWidgetItem(F_name);
  filt2cpp->setText(0, "Filter2.cpp");
  {
    pathTemplate = "@PluginName@/Code/@PluginName@Filters/";
    QString resourceTemplate(":/Template/Code/Filter/Filter.cpp.in");
    PMFilterGenerator* gen = new PMFilterGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString("Filter2.cpp"),
                                                    QString("Filter2"),
                                                    resourceTemplate,
                                                    filt2cpp,
                                                    this);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(false);
    m_FilterClasses.push_back(gen);
  }
#endif

  QTreeWidgetItem* sourceList = new QTreeWidgetItem(F_name);
  sourceList->setText(0, tr("SourceList.cmake"));

  QTreeWidgetItem* F_namefilters = new QTreeWidgetItem(F_doc);
  F_namefilters->setText(0, "Unknown Plugin Name");
  {
       pathTemplate = "@PluginName@/Documentation/@PluginName@Filters";
       QString resourceTemplate("");
       PMDirGenerator* gen = new PMDirGenerator(m_OutputDir->text(),
                                                       pathTemplate,
                                                       QString(""),
                                                       resourceTemplate,
                                                       F_namefilters,
                                                       this);
       gen->setDisplaySuffix("Filters");
       gen->setDoesGenerateOutput(false);
       gen->setNameChangeable(true);
       m_GenObjects.push_back(gen);
       connect(m_PluginName, SIGNAL(textChanged(const QString &)),
               gen, SLOT(pluginNameChanged(const QString &)));
     }


  QTreeWidgetItem* pluginDocs = new QTreeWidgetItem(F_doc);
  pluginDocs->setText(0, "Unknown Plugin Name");
  {
    pathTemplate = "@PluginName@/Documentation/";
    QString resourceTemplate(":/Template/Documentation/FilterDocs.qrc.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString(""),
                                                    resourceTemplate,
                                                    pluginDocs,
                                                    this);
    gen->setDisplaySuffix("PluginDocs.qrc");
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(true);
    m_GenObjects.push_back(gen);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
  }

  QTreeWidgetItem* htmlDoc = new QTreeWidgetItem(F_namefilters);
  htmlDoc->setText(0, "Unknown Plugin Name");
  {
    pathTemplate = "@PluginName@/Documentation/@PluginName@Filters/";
    QString resourceTemplate(":/Template/Documentation/Filter/Documentation.html.in");
    PMFileGenerator* gen = new PMFileGenerator(m_OutputDir->text(),
                                                    pathTemplate,
                                                    QString(""),
                                                    resourceTemplate,
                                                    htmlDoc,
                                                    this);
    gen->setDisplaySuffix("Filter.html");
    gen->setDoesGenerateOutput(true);
    gen->setNameChangeable(true);
    m_GenObjects.push_back(gen);
    connect(m_PluginName, SIGNAL(textChanged(const QString &)),
            gen, SLOT(pluginNameChanged(const QString &)));
    connect(m_OutputDir, SIGNAL(textChanged(const QString &)),
            gen, SLOT(outputDirChanged(const QString &)));
    // For "Directories" this probably isn't needed
    connect(generateButton, SIGNAL(clicked()),
            gen, SLOT(generateOutput()));
    connect(gen, SIGNAL(outputError(const QString &)),
            this, SLOT(generationError(const QString &)));
  }

  m_PluginName->setText("Unknown Plugin Name");
  treeWidget->expandAll();
  statusbar->showMessage("Ready");

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PluginMaker::on_selectButton_clicked() {
  QString outputFile = this->m_OpenDialogLastDirectory + QDir::separator();
  outputFile = QFileDialog::getExistingDirectory(this, tr("Select Directory"), outputFile);
  if (!outputFile.isNull())
  {
    m_OutputDir->setText(QDir::toNativeSeparators(outputFile));
    m_OpenDialogLastDirectory = outputFile;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PluginMaker::on_generateButton_clicked()
{
  QString pluginName = m_PluginName->text();
  QString pluginDir = m_OutputDir->text();

  if (pluginName == "") {
    statusbar->showMessage("Generation Failed --- Please provide a plugin name");
    QMessageBox::critical(this, tr("PluginMaker"), tr("The file generation was unsuccessful.\n"
      "Please enter a Plugin Name."));
    return;
  }
  else if (pluginDir == "") {
    statusbar->showMessage("Generation Failed --- Please provide a plugin directory");
    QMessageBox::critical(this, tr("PluginMaker"), tr("The file generation was unsuccessful.\n"
      "Please enter a Plugin Directory."));
    return;
  }

  // WE need to generate the SourceList.cmake file here because we possibly have
  // more than a single filter
  QString cmakeHdrCode("${@PluginName@Plugin_SOURCE_DIR}/Code/@PluginName@Filters/");
  cmakeHdrCode.replace("@PluginName@", m_PluginName->text());

  QString srcContents;
  QString hdrContents;
  for (int i = 0; i < m_FilterClasses.count(); ++i)
  {
    QFileInfo fi(m_FilterClasses[i]->getFileName());
    if (fi.suffix().compare("h") == 0)
    {
      hdrContents.append("    ").append(cmakeHdrCode).append(m_FilterClasses[i]->getFileName()).append("\n    ");
    }
    else if (fi.suffix().compare("cpp") == 0)
    {
      srcContents.append("    ").append(cmakeHdrCode).append(m_FilterClasses[i]->getFileName()).append("\n    ");
    }
    pluginName = m_FilterClasses[i]->getPluginName();
  }

  // Create File
  QFile rfile(":/Template/Code/Filter/SourceList.cmake.in");
  if ( rfile.open(QIODevice::ReadOnly | QIODevice::Text) )
  {
    QTextStream in(&rfile);
    QString text = in.readAll();
    text.replace("@PluginName@", pluginName);
    text.replace("@GENERATED_CMAKE_HEADERS_CODE@", hdrContents);
    text.replace("@GENERATED_CMAKE_SOURCE_CODE@", srcContents);
    QString pathTemplate = "@PluginName@/Code/@PluginName@Filters/";
    QString parentPath = m_OutputDir->text() + QDir::separator()
                        + pathTemplate.replace("@PluginName@", pluginName);
    parentPath = QDir::toNativeSeparators(parentPath);

    QDir dir(parentPath);
    dir.mkpath(parentPath);

    parentPath = parentPath + QDir::separator() + "SourceList.cmake";
    //Write to file
    QFile f(parentPath);
    if ( f.open(QIODevice::WriteOnly | QIODevice::Text) ) {
      QTextStream out(&f);
      out << text;
    }

  }




  statusbar->showMessage("Generation Completed");
}

void PluginMaker::processFile(QString path) {}

QString PluginMaker::cleanName(QString name) {
  //Remove all uses of "Plugin", "plugin", "Filter", and "filter"
  QRegExp rx("Plugin|plugin|Filter|filter");
  name.replace(rx, "");
  //Remove all spaces and illegal characters from plugin name
  name.trimmed();
  name.remove(" ");
  name.remove(QRegExp("[^a-zA-Z_\\d\\s]"));
  return name;
}


void PluginMaker::on_m_PluginName_textChanged(const QString & text) {
  statusbar->showMessage("Ready");

}

void PluginMaker::on_m_OutputDir_textChanged(const QString & text) {
  statusbar->showMessage("Ready");
}

void PluginMaker::on_helpButton_clicked() {
  HelpWidget* helpDialog = new HelpWidget;
  helpDialog->show();
}

void PluginMaker::on_aboutButton_clicked()

{
  ApplicationAboutBoxDialog about(PluginMakerProj::LicenseList, this);
  QString an = QCoreApplication::applicationName();
  QString version("");
  version.append("1.0.0");
  about.setApplicationInfo(an, version);
  about.exec();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PluginMaker::generationError(const QString& test)
{

}
