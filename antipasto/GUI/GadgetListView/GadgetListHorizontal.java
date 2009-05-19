package antipasto.GUI.GadgetListView;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.io.File;
import java.util.ArrayList;

import javax.swing.DefaultListModel;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.event.MouseInputListener;

import processing.app.Base;

import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.Interfaces.IModule;

public class GadgetListHorizontal extends JPanel implements ISelectedItemListener{
	
	File baseDirectory;
	ArrayList modules = new ArrayList();
	JScrollPane scrollPane = new JScrollPane();
	GadgetList gadgetList;
	GadgetPanel panel;
	JViewport viewPort;
	public GadgetListHorizontal(File gadgetLibDirectory, GadgetList gl, GadgetPanel gadgetPanel){
		super();
		
		panel = gadgetPanel;
		
		this.setBackground(Color.white);
		
		JPanel panel = new JPanel();
		panel.setBackground(Color.white);
		BorderLayout bl = new BorderLayout();
		panel.setSize(300, 60);
		panel.setPreferredSize(new Dimension(300, 60));
		panel.setLayout(bl);
		this.add(panel);
		
		gadgetList = gl;
		baseDirectory = gadgetLibDirectory;
		
		scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollPane.setSize(new Dimension(100, 60));
		scrollPane.setPreferredSize(new Dimension(100, 60));
		scrollPane.setBorder(null);
		
		this.viewPort = scrollPane.getViewport();
		
		panel.add(scrollPane, BorderLayout.CENTER);
		JLabel leftButton = new JLabel();
		
		Image leftImage = Base.getImage("module-library-left-arr.gif", this);
		ImageIcon leftIcon = new ImageIcon();
		leftIcon.setImage(leftImage);
		leftButton.setIcon(leftIcon);
		leftButton.setSize(15, 80);
		leftButton.addMouseListener(new MouseInputListener(){
			public void mouseClicked(MouseEvent arg0) {
				ScrollPanel(-40,0);
			}
			public void mouseEntered(MouseEvent arg0) {
			}
			public void mouseExited(MouseEvent arg0) {
			}
			public void mousePressed(MouseEvent arg0) {
			}
			public void mouseReleased(MouseEvent arg0) {
			}
			public void mouseDragged(MouseEvent arg0) {
			}
			public void mouseMoved(MouseEvent arg0) {
			}
		});
		
		panel.add(leftButton, BorderLayout.WEST);
		
		JLabel rightButton = new JLabel();
		
		Image rightImage = Base.getImage("module-library-right-arr.gif", this);
		ImageIcon rightIcon = new ImageIcon();
		rightIcon.setImage(rightImage);
		rightButton.setIcon(rightIcon);
		rightButton.setSize(15,80);
		rightButton.addMouseListener(new MouseInputListener(){
			public void mouseClicked(MouseEvent arg0) {
				ScrollPanel(40,0);
			}
			public void mouseEntered(MouseEvent arg0) {
			}
			public void mouseExited(MouseEvent arg0) {
			}
			public void mousePressed(MouseEvent arg0) {
			}
			public void mouseReleased(MouseEvent arg0) {
			}
			public void mouseDragged(MouseEvent arg0) {
			}
			public void mouseMoved(MouseEvent arg0) {
			}
		});
		panel.add(rightButton, BorderLayout.EAST);
		
		panel.setVisible(true);
		this.loadDirectory();
		
		scrollPane.setVisible(true);
	}

	/**
	 * Sets the internal gadget
	 * @param list Gadgetlist that we can add library modules to
	 */
	public void setGadgetList(GadgetList list){
		this.gadgetList = list;
	}
	
	/*
	 * loads all the modules from the module directory into the gadget panel
	 * */
	private void loadDirectory(){
		File[] files = this.baseDirectory.listFiles();
		ModuleFactory fact = new ModuleFactory();
		JPanel itemsPanel = new JPanel();
		itemsPanel.setBackground(Color.WHITE);
		itemsPanel.setLayout(new FlowLayout());
		itemsPanel.setSize(new Dimension(this.scrollPane.getWidth(), 70 ));
		File libDir = Base.createTempFolder("moduleLib");

		for(int i = 0; i < files.length; i ++){
			if(files[i].getName().endsWith(IModule.moduleExtension)){
				try {
					File curFile = files[i];
					IModule mod = fact.loadModule(curFile, libDir.getPath(), false);
					
					ModuleIcon label = new ModuleIcon(mod, this.gadgetList, this.panel);
					label.setSize(60,60);
					label.setPreferredSize(new Dimension(60,60));
					label.setVisible(true);
					label.addSelectionListener(this);
					
					itemsPanel.add(label);
					
					modules.add(label);
					
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		scrollPane.setSize(new Dimension(300, 60));
		scrollPane.setPreferredSize(new Dimension(300, 60));
		scrollPane.setViewportView(itemsPanel);
		itemsPanel.setVisible(true);		
	}

	public void onSelected(ModuleIcon selectedItem) {
		if(gadgetList.gadget != null){
			GadgetFactory fact = new GadgetFactory();
			try {
				fact.AddModuleToGadget(gadgetList.gadget, selectedItem.getModule());
				gadgetList.saveCurrentGadget();
				gadgetList.loadGadget(gadgetList.gadget);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	private void ScrollPanel(int xIncrement, int yIncrement){
		Point pt = viewPort.getViewPosition();
	    pt.x +=  xIncrement;
	    pt.y +=  yIncrement;

	    pt.x = Math.max(0, pt.x);
	    pt.x = Math.min(getMaxXExtent(), pt.x);
	    pt.y = Math.max(0, pt.y);
	    pt.y = Math.min(getMaxYExtent(), pt.y);

	    viewPort.setViewPosition(pt);
	}
	
	protected int getMaxXExtent() {
	    return viewPort.getView().getWidth() - viewPort.getWidth();
	  }

	  protected int getMaxYExtent() {
	    return viewPort.getView().getHeight() - viewPort.getHeight();
	  }

}
