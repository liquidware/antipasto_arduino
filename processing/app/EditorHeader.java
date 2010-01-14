/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
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
*/

package processing.app;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.ArrayList;

import javax.swing.*;
import javax.swing.event.*;


/**
 * Sketch tabs at the top of the editor window.
 */
public class EditorHeader extends JPanel {
  static Color backgroundColor;
  static Color textColor[] = new Color[2];

  Editor editor;

  int tabLeft[];
  int tabRight[];

  Font font;
  FontMetrics metrics;
  int fontAscent;

  JMenu menu;
  JPopupMenu popup;

  JMenuItem hideItem;

  int menuLeft;
  int menuRight;

  ArrayList tabs = new ArrayList();

  String previousSketch = "";

  static final String STATUS[] = { "unsel", "sel" };
  static final int UNSELECTED = 0;
  static final int SELECTED = 1;

  static final String WHERE[] = { "left", "mid", "right", "menu" };
  static final int LEFT = 0;
  static final int MIDDLE = 1;
  static final int RIGHT = 2;
  static final int MENU = 3;

  static final int PIECE_WIDTH = 4;

  Image[][] pieces;
  Image offscreen;
  int sizeW, sizeH;
  int imageW, imageH;
  JPanel tabHeader;

  public EditorHeader(Editor eddie) {

    this.editor = eddie; // weird name for listener

    pieces = new Image[STATUS.length][WHERE.length];
    for (int i = 0; i < STATUS.length; i++) {
      for (int j = 0; j < WHERE.length; j++) {
        pieces[i][j] = Base.getImage("tab-" + STATUS[i] + "-" +
                                        WHERE[j] + ".gif", this);
      }
    }

    if (backgroundColor == null) {
      //backgroundColor =
        //Preferences.getColor("header.bgcolor");
	  // hardcoding new blue color scheme for consistency with images,
	  // see EditorStatus.java for details.
      backgroundColor = new Color(0x21, 0x68, 0x86);
      textColor[SELECTED] =
        Preferences.getColor("header.text.selected.color");
      textColor[UNSELECTED] =
        Preferences.getColor("header.text.unselected.color");
    }

    //Setup the layout managers
    this.setBackground(backgroundColor);
		this.setLayout(new FlowLayout(FlowLayout.LEFT,0,5));
		this.setOpaque(true);

    rebuildHeaderBackground();

  }

  private void rebuildHeaderBackground() {

    //Existance check
    if (tabHeader != null) {
      this.remove(tabHeader);
    }

    // Ditch the old one, make a new one
    tabHeader = new JPanel(new FlowLayout());
    tabHeader.setPreferredSize(new Dimension(800, 40));
    tabHeader.setBackground(backgroundColor);
		tabHeader.setLayout(new FlowLayout(FlowLayout.LEFT,0,0));
		tabHeader.setOpaque(true);
    this.add(tabHeader);
  }

  private void createNewSketchTabs(Sketch sketch) {

	    for (int i = 0; i < sketch.code.length; i++) {

        SketchCode code = sketch.code[i];

        String codeName = (code.flavor == Sketch.PDE) ?
          code.name : code.file.getName();


        EditorTab editorTab = new EditorTab(new Color(0x54, 0x91, 0x9e), codeName, code, this.editor);
        editorTab.setPreferredSize(new Dimension( (codeName.length() * 7) + 10,
                                                 getHeight() + 5) );
        editorTab.setBackground(this.backgroundColor);

        tabs.add(editorTab);
        tabHeader.add(editorTab);
      }

  }

  private void createNewDataTabs() {

	  if(editor.gadgetPanel != null && editor.gadgetPanel.getActiveModule() != null){
		 if(editor.gadgetPanel.gadgetIsLoaded){
	    	if(editor.gadgetPanel.getActiveModule().getData() != null||editor.gadgetPanel.getActiveModule().getData().length > 0){
		    	EditorTab editorTab = new EditorTab(new Color(46, 163, 94), "Data", editor.gadgetPanel.getActiveModule(),this.editor);
		    	editorTab.setPreferredSize(new Dimension( ("Data".length() * 7) + 10,
                                                    this.getHeight() + 5));
		    	editorTab.setBackground(this.backgroundColor);
		    	tabs.add(editorTab);
		    	this.tabHeader.add(editorTab);
	    	}
		    }
	    }
  }

  private void rebuildTabs() {

    Sketch sketch = editor.sketch;

    tabs.clear();

    createNewSketchTabs(sketch);
    createNewDataTabs();
    this.previousSketch = sketch.name;
  }


  /**
   * Usually called when a new sketch is opened.
   */
  public void rebuild() {
    rebuildHeaderBackground();
    rebuildTabs();
    revalidate();
  }



  public void rebuildMenu() {
    //System.out.println("rebuilding");
    if (menu != null) {
      menu.removeAll();

    } else {
      menu = new JMenu();
      popup = menu.getPopupMenu();
      add(popup);
      popup.setLightWeightPopupEnabled(true);
    }
    JMenuItem item;

    // maybe this shouldn't have a command key anyways..
    // since we're not trying to make this a full ide..

    item = new JMenuItem("New Tab");
    final EditorHeader header = this;
    item.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          editor.sketch.newCode();
          header.paintComponents(header.getGraphics());
        }
      });
    menu.add(item);

    item = new JMenuItem("Rename");
    item.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          editor.sketch.renameCode();
          if (editor.sketch.current == editor.sketch.code[0]) {
            editor.sketchbook.rebuildMenus();
          }
        }
      });
    menu.add(item);

    item = new JMenuItem("Delete");
    item.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          editor.sketch.deleteCode();
        }
      });
    menu.add(item);

    item = new JMenuItem("Hide");
    item.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          editor.sketch.hideCode();
        }
      });
    menu.add(item);
    hideItem = item;

    JMenu unhide = new JMenu("Unhide");
    ActionListener unhideListener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
          String which = (String) e.getActionCommand();
          editor.sketch.unhideCode(which);
          rebuildMenu();
        }
      };
    Sketch sketch = editor.sketch;
    if (sketch != null) {
      for (int i = 0; i < sketch.hiddenCount; i++) {
        item = new JMenuItem(sketch.hidden[i].name +
          Sketch.flavorExtensionsShown[sketch.hidden[i].flavor]);
        item.setActionCommand(sketch.hidden[i].name +
          Sketch.flavorExtensionsShown[sketch.hidden[i].flavor]);
        item.addActionListener(unhideListener);
        unhide.add(item);
      }
    }
    if (unhide.getItemCount() == 0) {
      unhide.setEnabled(false);
    }

    menu.add(unhide);
    menu.addSeparator();

    int ctrlAlt = ActionEvent.ALT_MASK |
    Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    item = new JMenuItem("Previous Tab");
    KeyStroke ctrlAltLeft = KeyStroke.getKeyStroke(KeyEvent.VK_LEFT, ctrlAlt);
    item.setAccelerator(ctrlAltLeft);

    // this didn't want to work consistently

    menu.add(item);

    //item = Editor.newJMenuItem("Next Tab", ']', true);
    item = new JMenuItem("Next Tab");
    KeyStroke ctrlAltRight = KeyStroke.getKeyStroke(KeyEvent.VK_RIGHT, ctrlAlt);
    item.setAccelerator(ctrlAltRight);

    menu.add(item);

    if (sketch != null) {
      menu.addSeparator();

      ActionListener jumpListener = new ActionListener() {
          public void actionPerformed(ActionEvent e) {
            editor.sketch.setCurrent(e.getActionCommand());
          }
        };
      for (int i = 0; i < sketch.codeCount; i++) {
        item = new JMenuItem(sketch.code[i].name +
          Sketch.flavorExtensionsShown[sketch.code[i].flavor]);
        item.addActionListener(jumpListener);
        menu.add(item);
      }
    }
  }


  public void deselectMenu() {
    repaint();
  }

  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  public Dimension getMinimumSize() {
    if (Base.isMacOS()) {
      return new Dimension(300, Preferences.GRID_SIZE);
    }
    return new Dimension(300, Preferences.GRID_SIZE - 1);
  }

  public Dimension getMaximumSize() {
    if (Base.isMacOS()) {
      return new Dimension(3000, Preferences.GRID_SIZE);
    }
    return new Dimension(3000, Preferences.GRID_SIZE - 1);
  }


}
