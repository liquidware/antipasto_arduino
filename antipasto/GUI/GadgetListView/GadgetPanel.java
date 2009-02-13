package antipasto.GUI.GadgetListView;

import javax.imageio.ImageIO;
import javax.swing.event.EventListenerList;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.*;

import antipasto.CoreFactory;
import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.GUI.GadgetListView.AddGadgetMenu;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.ActiveGadgetObject;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.SketchChangingObject;
import antipasto.Interfaces.IGadget;
import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IPackedFile;
import antipasto.Interfaces.ITemporary;

import processing.app.Base;
import processing.app.Editor;
import processing.app.Serial;

import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.net.URL;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;

public class GadgetPanel extends JDialog implements ListSelectionListener, IActiveSketchChangingListener, ComponentListener,
													MouseListener 
													{

 
    //JList list;
    private EventListenerList activeGadgetChangedEventList = new EventListenerList();
    private EventListenerList sketchBookChangedEventList = new EventListenerList();

    private GadgetFactory skbFact = new GadgetFactory();
    private CoreFactory coreFact= new CoreFactory();
    private ModuleFactory modFact = new ModuleFactory();
    private GadgetList list;
    private Serial serial;

    private File _sketchBookDirectory;

    private IModule activeModule;

    private IGadget _gadget;

    public GadgetPanel(String sketchBookDirectory) {
        this.addMouseListener(new ModuleMouseAdapter());
        this.setUndecorated(true);
    }
    
    public void loadGadget(String gadget){
    	this.loadGadget(new File(gadget));
    }
    
    public void loadGadget(File gadget){
    	if(gadget  != null){
	    	this.getContentPane().setLayout(new BorderLayout());
	    	
	    	GadgetFactory fact = skbFact;
	    	
	        String dir = System.getProperty("java.io.tmpdir") + File.separator + gadget.getName();
	        IGadget book = fact.loadGadget(gadget, dir);

	        list = new GadgetList(book, gadget.getParent());
	    	//Save this for later!
	        GadgetListHorizontal libPanel = new GadgetListHorizontal(new File("C:" + File.separator + "OpenHardware" + File.separator + "Modules"), list);
	    	JPanel top = new JPanel();
	        top.setBackground(new Color(0x04, 0x4F, 0x6F));
	    	this.getContentPane().add(top, BorderLayout.NORTH);
	    	top.setSize(new Dimension(this.getWidth(), 15));
	        top.setLayout(new FlowLayout());
	        
	        try {
	        	ImageIcon upperRight = new ImageIcon("..\\lib\\upperleftgadget.png");
	    		upperRight.setImage(Base.getImage("..\\lib\\upperleftgadget.png", this));
	    		JLabel upperImageLabel = new JLabel(upperRight);
				top.add(upperImageLabel);
				upperImageLabel.setVisible(true);
			} catch (Exception e) {
				e.printStackTrace();
			}
	        
	    	JPanel bottom = new JPanel();
	    	bottom.setBackground(new Color(0x04, 0x4F, 0x6F));
	    	this.getContentPane().add(bottom, BorderLayout.SOUTH);
	    	bottom.setSize(new Dimension(this.getWidth(), 15));
	    	bottom.setLayout(new FlowLayout());
	    	ImageIcon lowerRight = new ImageIcon();
	    	try {
	    		ImageIcon lowerRightImage = new ImageIcon("\\lib\\upperleftgadget.png");
				JLabel lowerImageLabel = new JLabel(lowerRightImage);
				top.add(lowerImageLabel);
				lowerImageLabel.setVisible(true);
			} catch (Exception e) {
				e.printStackTrace();
			}
	        
	        
	    	
	    	JPanel right = new JPanel();
	    	right.setBackground(new Color(0x04, 0x4F, 0x6F));
	    	this.getContentPane().add(right, BorderLayout.WEST);
	    	right.setSize(new Dimension(15, this.getHeight()));
	        
	    	
	    	JButton button;
	        
	        System.out.println("Temporary directory is = " + dir);
	
	        _gadget = book;
	
	        //GadgetWindowTransferHandler transferHandler = new GadgetWindowTransferHandler();
	        list.addSketchChangingeListener(this);
	
	        list.addListSelectionListener(this);
	        Box box = Box.createVerticalBox();
	        
	        JScrollPane scrollPanel = new JScrollPane((JList) list);
	        scrollPanel.setPreferredSize(new Dimension(300, 300));
	        scrollPanel.setSize(new Dimension(300, 300));
	        
	        libPanel.setSize(300, 100);
	        libPanel.setPreferredSize(new Dimension(300, 100));
	        libPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
	        
	        box.setBackground(new Color(0x04, 0x4F, 0x6F));
	        
	        box.add(libPanel);
	        box.add(scrollPanel);
	
	        this.getContentPane().add(box, BorderLayout.CENTER);
	        
	        //JPanel panel = new JPanel();
	        //panel.setLayout(new FlowLayout());
	        //panel.setSize(200, 100);
	
	        /*button = new JButton("Add Gadget");
	        button.addMouseListener(new AddGadgetMenu(menu, button));
	
	        panel.add(button);
	        button = new JButton("Remove Gadget");
	        panel.add(button);
	        */
	        //this.getContentPane().add(panel, BorderLayout.PAGE_START);
	        //panel.setVisible(true);
	
	        libPanel.setVisible(true);
	        right.setVisible(true);
	        top.setVisible(true);
	        bottom.setVisible(true);
	        this.setVisible(true);
    	}else{
    		this.setVisible(false);
    		this.hide();				//gotta upgrade the java version....
    	}
    }

    /*
    * Called when the active listview is changed
    * */
    public void valueChanged(ListSelectionEvent e) {
        GadgetList list = (GadgetList) e.getSource();
        if(list.getSelectedValue() instanceof IModule){
            if(this.activeModule != list.getSelectedValue()){
                this.activeModule = (IModule) list.getSelectedValue();
                this.onActiveGadgetChange(new ActiveGadgetObject(this,
                        this.activeModule.getSketchFile(),
                        this.activeModule.getBoardsFile()));
            }
        }
    }

    public void addActiveGadgetChangedEventListener(IActiveGadgetChangedEventListener listener){
        this.activeGadgetChangedEventList.add(IActiveGadgetChangedEventListener.class, listener);
    }

    public void removeActiveGadgetChangedEventListener(IActiveGadgetChangedEventListener listener){
        this.activeGadgetChangedEventList.remove(IActiveGadgetChangedEventListener.class, listener);
    }

    private void onActiveGadgetChange(ActiveGadgetObject evObj){
         Object[] listeners = activeGadgetChangedEventList.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== IActiveGadgetChangedEventListener.class) {
                            ((IActiveGadgetChangedEventListener)listeners[i+1]).onActiveGadgetChanged(evObj);
                        }
                    }
    }

    public void addSketchBookChangedEventListener(IActiveSketchChangingListener listener){
        this.sketchBookChangedEventList.add(IActiveSketchChangingListener.class, listener);
    }

     public void removeSketchBookChangedEventListener(IActiveSketchChangingListener listener){
        this.sketchBookChangedEventList.remove(IActiveSketchChangingListener.class, listener);
     }

    public void saveCurrentSketch(){
    	try{
    		if(list.gadget != null){
    			list.saveCurrentGadget();
    		}
    	}catch(Exception ex){
    	}
    }

    public void deleteCurrentSketchOutputDirectory() throws Exception {
        if(this._gadget instanceof ITemporary){
            File dir = new File(((ITemporary)_gadget).getTempDirectory());
            if(!dir.delete()){
                throw new Exception("New sketch output");
            }
        }
    }

    public Serial getSerial(){
    	return this.serial;
    }
    
    public void setSerial(Serial serial){
    	this.serial = serial;
    }
    
    public void loadSketchFile(File f){
        this.skbFact.loadGadget(f, System.getProperty("java.io.tmpdir") + File.separator + dropExtension(f.getName()));
    }

    public void onActiveSketchChanged(SketchChangingObject obj) {
        this.saveCurrentSketch();
        this.loadSketchFile(obj.newFile);
    }

    public String dropExtension(String fileName){
        return fileName.split(".")[0];
    }

    public IGadget getActiveGadget(){
    	if(this._gadget == null){
    		return null;
    	}
    	
        IModule[] gadgets = _gadget.getModules();
        for(int i=0; i < gadgets.length; i++){
            IModule gadget = gadgets[i];
            File boardsFile = gadget.getBoardsFile();
            File sketchFile = gadget.getSketchFile();
        }
        return this._gadget;
    }
    
    public IModule getActiveModule(){
    	if(this.activeModule == null){
    		if(this._gadget != null){
    			this.activeModule = this._gadget.getModules()[0];
    		}else{
    			return null;
    		}
    	}
    	return this.activeModule;
    }

	public void componentHidden(ComponentEvent arg0) {
		this.hide();
	}

	public void componentMoved(ComponentEvent arg0) {
		Editor editor = (Editor) arg0.getComponent();
		//int width = 300;
        //this.setSize(width, editor.textarea.getHeight());
        //this.setLocation(editor.getX() - this.getWidth(), editor.textarea.getLocationOnScreen().y);	
	}

	public void componentResized(ComponentEvent arg0) {
		Editor editor = (Editor) arg0.getComponent();
		int width = 300;
        this.setSize(width, editor.textarea.getHeight());
        this.setLocation(editor.getX() - this.getWidth(), editor.textarea.getLocationOnScreen().y);	
		
	}

	public void componentShown(ComponentEvent arg0) {
		this.show();
	}
	
	public void closeActiveGadget(){
		this._gadget = null;
	}

	public void mouseClicked(MouseEvent e) {	
	}
	public void mouseEntered(MouseEvent e) {
	}
	public void mouseExited(MouseEvent e) {
	}
	public void mousePressed(MouseEvent e) {
	}
	public void mouseReleased(MouseEvent e) {
	}
}


     


