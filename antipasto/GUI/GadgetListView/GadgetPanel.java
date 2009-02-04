package antipasto.GUI.GadgetListView;

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

import processing.app.Editor;
import processing.app.Serial;

import java.io.File;
import java.io.IOException;
import java.awt.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.ArrayList;

public class GadgetPanel extends JWindow implements ListSelectionListener, IActiveSketchChangingListener, ComponentListener {

 
    //JList list;
    private EventListenerList activeGadgetChangedEventList = new EventListenerList();
    private EventListenerList sketchBookChangedEventList = new EventListenerList();

    private GadgetFactory skbFact = new GadgetFactory();
    private CoreFactory coreFact= new CoreFactory();
    private ModuleFactory gadgFact = new ModuleFactory();
    private GadgetList list;
    private Serial serial;

    private File _sketchBookDirectory;

    private IModule activeModule;

    private IGadget _gadget;

    public GadgetPanel(String sketchBookDirectory) {
        
    }
    
    public void loadGadget(String gadget){
    	this.loadGadget(new File(gadget));
    }
    
    public void loadGadget(File gadget){
    	if(gadget  != null){
	    	this.getContentPane().setLayout(new BorderLayout());
	    	JPanel top = new JPanel();
	    	top.setBackground(Color.GREEN);
	    	this.getContentPane().add(top, BorderLayout.NORTH);
	    	
	        JButton button;
	        GadgetFactory fact = skbFact;
	        String dir = System.getProperty("java.io.tmpdir") + File.separator + gadget.getName();
	        IGadget book = fact.loadGadget(gadget, dir);
	
	        System.out.println("Temporary directory is = " + dir);
	
	        _gadget = book;
	
	        GadgetWindowTransferHandler transferHandler = new GadgetWindowTransferHandler();
	        list = new GadgetList(book, gadget.getParent());
	        list.addSketchChangingeListener(this);
	
	        list.addListSelectionListener(this);
	        JScrollPane scrollPanel = new JScrollPane((JList) list);
	        this.getContentPane().add(scrollPanel, BorderLayout.CENTER);
	
	        JPopupMenu menu = new JPopupMenu();
	
	        JPanel panel = new JPanel();
	        panel.setLayout(new FlowLayout());
	        panel.setSize(200, 100);
	
	        /*button = new JButton("Add Gadget");
	        button.addMouseListener(new AddGadgetMenu(menu, button));
	
	        panel.add(button);
	        button = new JButton("Remove Gadget");
	        panel.add(button);
	        */
	        this.getContentPane().add(panel, BorderLayout.PAGE_START);
	        panel.setVisible(true);
	
	        top.setVisible(true);
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
		// TODO Auto-generated method stub
		
	}

	public void componentMoved(ComponentEvent arg0) {
		Editor editor = (Editor) arg0.getComponent();
        this.setSize(this.getWidth(), editor.textarea.getHeight());
        this.setLocation(editor.getX() - this.getWidth(), editor.textarea.getLocationOnScreen().y);	
	}

	public void componentResized(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	public void componentShown(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	
	public void closeActiveGadget(){
		this._gadget = null;
	}
}


     


