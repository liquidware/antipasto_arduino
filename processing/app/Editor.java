/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Editor - main editor panel for the processing development environment
  Part of the Processing project - http://processing.org

  Copyright (c) 2004-05 Ben Fry and Casey Reas
  Copyright (c) 2001-04 Massachusetts Institute of Technology

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  $Id: Editor.java 370 2008-01-19 16:37:19Z mellis $
*/

package processing.app;

import processing.app.syntax.*;
import processing.app.tools.*;
import processing.core.*;

import java.awt.*;
import java.awt.datatransfer.*;
import java.awt.dnd.*;
import java.awt.event.*;
import java.awt.print.*;
import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import java.util.zip.*;

import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.undo.*;

import antipasto.*;
import antipasto.GUI.GadgetListView.*;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.*;
import antipasto.GUI.ImageListView.ImageListPanel;
import antipasto.Interfaces.*;
import antipasto.ModuleRules.IMessage;
import antipasto.ModuleRules.IOkListener;
import antipasto.Util.AntipastoFileFilter;
import antipasto.Util.GadgetFileFilter;
import antipasto.Util.PDEFileFilter;
import antipasto.Util.Utils;
import antipasto.Plugins.Manager.PluginPanel;
import antipasto.Plugins.*;
import antipasto.Plugins.Events.EditorEvent;

import com.apple.mrj.*;
import com.oroinc.text.regex.*;
//import de.hunsicker.jalopy.*;

import com.apple.mrj.*;
import gnu.io.*;

import org.arduino.tools.AntRunner;

public class Editor extends JFrame
implements MRJAboutHandler, MRJQuitHandler, MRJPrefsHandler,
MRJOpenDocumentHandler, IActiveGadgetChangedEventListener { //, MRJOpenApplicationHandler
    // yeah
    static final String WINDOW_TITLE = "Antipasto Arduino";

    // p5 icon for the window
    Image icon;

    // otherwise, if the window is resized with the message label
    // set to blank, it's preferredSize() will be fukered
    static public final String EMPTY =
    "                                                                     " +
    "                                                                     " +
    "                                                                     ";

    static public final KeyStroke WINDOW_CLOSE_KEYSTROKE =
    KeyStroke.getKeyStroke('W', Toolkit.getDefaultToolkit().getMenuShortcutKeyMask());

    static final String CODEEDITOR = "CODEPANEL";
    static final String FILELIST = "FILELIST";
    static final String BLANK = "BLANK";
    static final String TEST = "TEST";

    static final int HANDLE_NEW  = 1;
    static final int HANDLE_OPEN = 2;
    static final int HANDLE_QUIT = 3;
    int checkModifiedMode;
    String handleOpenPath;
    boolean handleNewShift;
    boolean handleNewLibrary;

    boolean gadgetIsLoading = false;

    PageFormat pageFormat;
    PrinterJob printerJob;

    EditorButtons buttons;
    EditorHeader header;
    EditorStatus status;
    EditorConsole console;
    Serial serialPort;
    JSplitPane splitPane;
    JPanel consolePanel;

    JLabel lineNumberComponent;

    JPanel leftWing;
    JPanel rightWing;
    JLabel leftExpandLabel;

    // currently opened program
    public Sketch sketch;

    public String lastActiveGadgetPath;

    EditorLineStatus lineStatus;
    String curBoard;

    public JEditTextArea textarea;
    EditorListener listener;

    // runtime information and window placement
    Point appletLocation;
    //Point presentLocation;
    //Window presentationWindow;
    RunButtonWatcher watcher;
    //Runner runtime;


    JMenuItem exportAppItem;
    JMenuItem saveMenuItem;
    JMenuItem saveAsMenuItem;
    JMenuItem newGadgetMenuItem;
    JMenuItem openMenuItem;
    public JPanel centerPanel;

    JMenuItem burnBootloader8Item = null;
    JMenuItem burnBootloader8ParallelItem = null;
    JMenuItem burnBootloader168DiecimilaItem = null;
    JMenuItem burnBootloader168DiecimilaParallelItem = null;
    JMenuItem burnBootloader168NGItem = null;
    JMenuItem burnBootloader168NGParallelItem = null;

    JMenu boardsMenu;

    JMenu serialMenu;
    JMenu serialRateMenu;
    JMenu mcuMenu;

    public ImageListPanel imageListPanel;

    SerialMenuListener serialMenuListener;

    boolean running;
    boolean presenting;
    boolean debugging;

    boolean isExporting = false;

    // undo fellers
    JMenuItem undoItem, redoItem;
    protected UndoAction undoAction;
    protected RedoAction redoAction;
    UndoManager undo;
    // used internally, and only briefly
    CompoundEdit compoundEdit;

    //Used specifically for the board files history
    File originalBoardsFile;
    File newBoardsFile;

    //SketchHistory history;  // TODO re-enable history
    Sketchbook sketchbook;
    //Preferences preferences;
    FindReplace find;
    public JFrame _frame;
    public GadgetPanel gadgetPanel ;
    public Wing referencePanel;
    ButtonGroup boardGroup = new ButtonGroup();

    private EventListenerList activeBoardChangedEventList = new EventListenerList();


    public PluginPanel pluginPanel = new PluginPanel(Base.pluginloader);
    AntRunner ant = new AntRunner();
    //static Properties keywords; // keyword -> reference html lookup


    public Editor() {
        super();
        _frame = this;
        this.setTitle(WINDOW_TITLE);
        // #@$*(@#$ apple.. always gotta think different
        MRJApplicationUtils.registerAboutHandler(this);
        MRJApplicationUtils.registerPrefsHandler(this);
        MRJApplicationUtils.registerQuitHandler(this);
        MRJApplicationUtils.registerOpenDocumentHandler(this);

        // run static initialization that grabs all the prefs
        Preferences.init();

        // set the window icon
        try {
            icon = Base.getImage("icon.gif", this);
            _frame.setIconImage(icon);
        } catch (Exception e) {
        } // fail silently, no big whup


        // add listener to handle window close box hit event
        _frame.addWindowListener(new WindowAdapter() {
                                     public void windowClosing(WindowEvent e) {
                                         handleQuitInternal();
                                     }
                                 });
        // don't close the window when clicked, the app will take care
        // of that via the handleQuitInternal() methods
        // http://dev.processing.org/bugs/show_bug.cgi?id=440
        _frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);

        PdeKeywords keywords = new PdeKeywords();
        sketchbook = new Sketchbook(this);

        JMenuBar menubar = new JMenuBar();
        menubar.add(buildFileMenu());
        menubar.add(buildEditMenu());
        menubar.add(buildSketchMenu());
        menubar.add(buildToolsMenu());
        // what platform has their help menu way on the right? motif?
        //menubar.add(Box.createHorizontalGlue());
        menubar.add(buildHelpMenu());

        _frame.setJMenuBar(menubar);

        buildGadgetPanel();
        // doesn't matter when this is created, just make it happen at some point
        //find = new FindReplace(Editor.this);

        //Container pain = getContentPane();
        //pain.setLayout(new BorderLayout());
        // for rev 0120, placing things inside a JPanel because
        //Container contentPain = getContentPane();
        this.getContentPane().setLayout(new BorderLayout());
        JPanel pain = new JPanel();
        pain.setLayout(new BorderLayout());

        this.getContentPane().add(pain, BorderLayout.CENTER);


        Box box = Box.createVerticalBox();
        Box upper = Box.createVerticalBox();

        JPanel editorSection = new JPanel(new BorderLayout());

        buttons = new EditorButtons(this);
        upper.add(buttons);


        header = new EditorHeader(this);
        //header.setBorder(null);
        upper.add(header);


        textarea = new JEditTextArea(new PdeTextAreaDefaults());
        textarea.setRightClickPopup(new TextAreaPopup());
        //textarea.setTokenMarker(new PdeKeywords());
        textarea.setHorizontalOffset(6);

        // assemble console panel, consisting of status area and the console itself
        consolePanel = new JPanel();
        consolePanel.setLayout(new BorderLayout());

        status = new EditorStatus(this);
        consolePanel.add(status, BorderLayout.NORTH);

        console = new EditorConsole(this);
        // windows puts an ugly border on this guy
        console.setBorder(null);
        consolePanel.add(console, BorderLayout.CENTER);

        lineStatus = new EditorLineStatus(textarea);
        consolePanel.add(lineStatus, BorderLayout.SOUTH);

        leftExpandLabel = new JLabel("<");
        leftWing = new JPanel();
        leftWing.setBackground(new Color(0x54, 0x91, 0x9e));
        leftWing.setOpaque(true);
        leftWing.setSize(15, 0);
        leftWing.setPreferredSize(new Dimension(10, 0));
        leftWing.setLayout(new BorderLayout());
        leftWing.add(leftExpandLabel, BorderLayout.CENTER);



        leftWing.addMouseListener(new MouseListener() {
                                      public void mouseClicked(MouseEvent arg0) {

                                          gadgetPanel.setVisible(!gadgetPanel.isVisible());

                                          /* Handle the expand icon */
                                          if (gadgetPanel.isVisible()) {
                                              leftExpandLabel.setText(">");
                                          } else {
                                              leftExpandLabel.setText("<");
                                          }
                                      }
                                      public void mouseEntered(MouseEvent arg0) {
                                      }
                                      public void mouseExited(MouseEvent arg0) {
                                          // TODO Auto-generated method stub
                                      }
                                      public void mousePressed(MouseEvent arg0) {
                                          // TODO Auto-generated method stub
                                      }
                                      public void mouseReleased(MouseEvent arg0) {
                                          // TODO Auto-generated method stub
                                      }
                                  });

        imageListPanel = new ImageListPanel(this.gadgetPanel, new FlashTransfer());

        this.getContentPane().validate();

        JPanel testPanel = new JPanel();
        JLabel lbl = new JLabel("THIS IS A TEST STRING");
        lbl.setVisible(true);
        lbl.setBackground(Color.BLUE);


        centerPanel = new JPanel();

        Dimension dim = textarea.getSize();

        centerPanel.setLayout(new CardLayout());


        centerPanel.setVisible(true);
        centerPan