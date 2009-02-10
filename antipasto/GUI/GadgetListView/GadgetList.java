package antipasto.GUI.GadgetListView;

import java.io.*;

import javax.swing.*;
import javax.swing.event.EventListenerList;
import javax.swing.event.ListDataListener;
import javax.swing.event.ListDataEvent;

import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.SketchChangingObject;
import antipasto.Interfaces.IGadget;
import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IPackedFile;
import antipasto.Interfaces.ITemporary;

import java.io.*;
import java.util.Arrays;

public class GadgetList extends JList implements IGadgetWindowTransferFileEventListener {

    IGadget gadget;
    JList jList;
    GadgetCollection _collection;
    private String gadgetDirectory;

    private EventListenerList activeGadgetChangedEventList = new EventListenerList();
    private EventListenerList activeSketchChangedEventList = new EventListenerList();

    public GadgetList(IGadget gadget, String gadgetDirectory){
        super();
        this.loadGadget(gadget);
        this.gadgetDirectory = gadgetDirectory;
    }

    public GadgetList(String gajDirectory){
        super();
        this.gadgetDirectory = gajDirectory;
    }

    public void loadGadget(IGadget gaj){
        this.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        gadget = gaj;
        _collection = new GadgetCollection(gaj);
        
        GadgetWindowTransferHandler transferHandler = new GadgetWindowTransferHandler();
        transferHandler.addTransferFileListener(this);
        this.setModel(new GadgetListModel(_collection));
        this.setCellRenderer(new GadgetCellRenderer());
        this.setTransferHandler(transferHandler);
    }

    public void addGadgetToCurrentSketchBook(File f) {
        if(f.getName().endsWith(".module")){
            ModuleFactory fact = new ModuleFactory();
            try {
                this.loadGadget(new GadgetFactory().AddModuleToGadget(gadget, fact.loadModule(f, System.getProperty("java.io.tmpdir"), true)));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void loadGadget(File f) {
        if(f.getName().endsWith(".pde")){
            this.fireSketchChangingEvent(this.gadget, f);
        }
    }

    public void addActiveGadgetChangeEventListener(IActiveGadgetChangedEventListener listener) {
        this.activeGadgetChangedEventList.add(IActiveGadgetChangedEventListener.class, listener);
    }

    public void removeActiveGadgetChangeEventListener(IActiveGadgetChangedEventListener listener) {
        this.activeGadgetChangedEventList.remove(IActiveGadgetChangedEventListener.class, listener);
    }

    public void onFileTransfer(FileTransferObject ftObj) {
        File f = ftObj.getFile();

        if (f.getName().endsWith(IModule.moduleExtension)) {
            this.addGadgetToCurrentSketchBook(f);
        } else if (f.getName().endsWith(".pde") || f.getName().endsWith(".gadget")){
            this.loadGadget(f);
        }
    }

    public void closeCurrentSketch(){
        this.saveCurrentGadget();
    }

    public void saveCurrentGadget(){
        GadgetFactory factory = new GadgetFactory();
        try{
            //factory.writeSketchBookToFile(_book, sketchBookDirectory);
            String originalDirectory = ((IPackedFile)gadget).getPackedFile().getPath();
            String bookFileName = ((IPackedFile)gadget).getPackedFile().getName();
            antipasto.Interfaces.ITemporary temp = (antipasto.Interfaces.ITemporary)gadget;
            IModule[] newGadgets = new IModule[gadget.getModules().length];
            for(int i = 0; i < gadget.getModules().length; i++){
                IModule curGadget = gadget.getModules()[i];
                if(curGadget instanceof IPackedFile && curGadget instanceof ITemporary){
                    File gadgetDirectory = new File(((ITemporary)curGadget).getTempDirectory());
                    File packedFile = ((IPackedFile)curGadget).getPackedFile();

                    ModuleFactory fact = new ModuleFactory();
                    try{
                    	fact.WriteModuleToFile(curGadget, packedFile.getPath().substring(0,
                    			packedFile.getPath().length() - packedFile.getName().length()));
                    }catch(Exception ex){
                        ex.printStackTrace();
                    }
                    newGadgets[i] = curGadget;
                }
            }
            gadget.setModule(newGadgets);
            File tempDirectory = new File(temp.getTempDirectory());
            int endIndex = originalDirectory.length() - bookFileName.length();
            originalDirectory = originalDirectory.substring(0, endIndex);
            factory.writeGadgetToFile(gadget, originalDirectory);
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    public String getSketchBookDirectory() {
        return gadgetDirectory;
    }

    public void setSketchBookDirectory(String sketchBookDirectory) {
        this.gadgetDirectory = sketchBookDirectory;
    }

    public void addSketchChangingeListener(IActiveSketchChangingListener listener){
        this.activeSketchChangedEventList.add(IActiveSketchChangingListener.class, listener);
    }

    public void removeSketchChangingListener(IActiveSketchChangingListener listener){
        this.activeSketchChangedEventList.remove(IActiveSketchChangingListener.class, listener);
    }

    private void fireSketchChangingEvent(IGadget oldSketch, File newFile){
        Object[] listeners = activeSketchChangedEventList.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== IActiveSketchChangingListener.class) {
                            ((IActiveSketchChangingListener)listeners[i+1]).onActiveSketchChanged(new SketchChangingObject(oldSketch, newFile));
                        }
                    }
    }
}
