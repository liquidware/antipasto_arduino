package antipasto.Plugins;

import org.apache.log4j.Logger;
import org.java.plugin.ObjectFactory;
import org.java.plugin.PluginManager;
import org.java.plugin.PluginManager.PluginLocation;
import org.java.plugin.standard.StandardPluginLocation;
import org.java.plugin.registry.Extension;
import org.java.plugin.registry.ExtensionPoint;
import org.java.plugin.registry.PluginDescriptor;
import org.java.plugin.util.ExtendedProperties;

import java.util.*;
import java.io.*;

import antipasto.Plugins.Manager.PluginListModel;
import antipasto.Plugins.Manager.PluginPanel;
import antipasto.Plugins.Events.EditorEvent;
import antipasto.Plugins.Events.EventSender;
import antipasto.Plugins.Interfaces.EditorListener;

import processing.app.Base;

public class PluginLoader {

    public static final File pluginsDir = new File("plugins");
    private PluginManager pluginManager = null;
    public EventSender eventsender = new EventSender();
    private static final String LINE_SEP = System.getProperty("line.separator");
    private Logger logger;
    private PluginPanel pluginPanel;

    public PluginLoader() {
        prepareLoggers();
        initPluginManager();
        loadPlugins();
        startPlugins();
        setEventSender(eventsender);
        pluginPanel = new PluginPanel(this);
        //broadcasting pluginsLoaded
        getEventSender().broadcast(new EditorEvent(new EditorContext(Base.getActiveEditor()),2000));

    }

    public void showPluginPanel(){
        pluginPanel.showPluginPanel();
    }

    public PluginManager setManager(PluginManager pluginManager) {
        return this.pluginManager = pluginManager;
    }

    public PluginManager getManager() {
        return this.pluginManager;
    }
    public EventSender setEventSender(EventSender sender) {
        return this.eventsender = sender;
    }

    public EventSender getEventSender() {
        return this.eventsender;
    }

    private void prepareLoggers(){
        logger = Logger.getLogger(PluginLoader.class);
        LogOutputStream outStream = new LogOutputStream();
        PrintStream sysOut = new PrintStream(outStream, true);
        System.setOut(sysOut);
    }

    private void initPluginManager() {
        try {
            ExtendedProperties ep = new ExtendedProperties();
            ep.put("org.java.plugin.PathResolver", "org.java.plugin.standard.ShadingPathResolver");
            ep.put("unpackMode", "always");
            ep.put("org.java.plugin.standard.ShadingPathResolver.unpackMode", "always");
            pluginManager = ObjectFactory.newInstance(ep).createManager(
                    ObjectFactory.newInstance(ep).createRegistry(),
                    ObjectFactory.newInstance(ep).createPathResolver());

            setManager(pluginManager);

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    public void loadPlugins() {
        try {
            File[] plugins = pluginsDir.listFiles(new FilenameFilter() {

                public boolean accept(File dir, String name) {
                    return name.toLowerCase().endsWith(".zip");
                }
            });

            PluginLocation[] locations = new PluginLocation[plugins.length];

            for (int i = 0; i < plugins.length; i++) {
                locations[i] = StandardPluginLocation.create(plugins[i]);
                System.out.println(plugins[i] + " loaded");
            }

            System.out.println(plugins.length + " plugins loaded");

            getManager().publishPlugins(locations);

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }

    }

    private void startPlugins() {
        //try {
            PluginDescriptor core = getManager().getRegistry().getPluginDescriptor("com.plugin.core");
            ExtensionPoint point = getManager().getRegistry().getExtensionPoint(core.getId(), "Section");

            for (Iterator it = point.getConnectedExtensions().iterator(); it.hasNext();) {
            	Extension ext = (Extension) it.next();
            	initializePlugin(ext);
            }

        //} catch (PluginLifecycleException e) {
        //    System.out.println("ERROR starting plugin: " + e.getMessage());
        //}

    }

    public void initializePlugin(Extension ext){
        try{
        System.out.println("activating: " + ext);
        PluginDescriptor descr = ext.getDeclaringPluginDescriptor();

        PluginBase plug = (PluginBase) getManager().getPlugin(descr.getId());
        plug.postInit();

      try{
        ClassLoader classLoader = getManager().getPluginClassLoader(descr);
        Class<?> pclass= classLoader.loadClass(ext.getParameter("class").valueAsString());
        getEventSender().addEventListener((EditorListener) plug,pclass);
      }catch(Exception e){
    	  System.out.println("ERROR starting plugin: " + e.getMessage());
      }
        }catch(Exception e){
      	  System.out.println("ERROR starting plugin: " + e.getMessage());
        }

    }

    public PluginListModel listRegisteredPlugins() {
        PluginListModel listModel = new PluginListModel();
        try {
            PluginDescriptor core = getManager().getRegistry().getPluginDescriptor("com.plugin.core");
            ExtensionPoint point = getManager().getRegistry().getExtensionPoint(core.getId(), "Section");

            for (Iterator it = point.getConnectedExtensions().iterator(); it.hasNext();) {
                Extension ext = (Extension) it.next();
                System.out.println("listing: " + ext);
                PluginDescriptor descr = ext.getDeclaringPluginDescriptor();


                //ClassLoader classLoader = getManager().getPluginClassLoader(descr);
                //Class<?> pclass= classLoader.loadClass(ext.getParameter("class").valueAsString());
                //PluginBase psec = (PluginBase) pclass.newInstance();


                listModel.addElement(descr, ext.getParameter("class").valueAsString());
            }

        } catch (Exception e) {
            System.out.println("ERROR listing plugins: " + e.getMessage());
            return listModel;
        }
        return listModel;
    }
    /**************

    PluginDescriptor[] plugins = new PluginDescriptor[extensions.size()];

    final String jpfPluginID = ext.getDeclaringPluginDescriptor().getId();
    final String pluginID = "12"
    final BeenPluginDescriptor desc = new PluginDescriptor(pluginID);
    desc.setName(getPluginName(pluginID));
    if (ext.getDeclaringPluginDescriptor().getDocumentation() != null) {
    desc.setDescription(ext.getDeclaringPluginDescriptor().getDocumentation().getText());
    }
    String pluginVersion = ext.getDeclaringPluginDescriptor().getVersion().toString();
    if (! StringUtils.isEmpty(pluginVersion)) {
    desc.setVersion(pluginVersion);
    }

    plugins[i++] = desc;

    DefaultPluginsCollector collector = new DefaultPluginsCollector();
    try {
    collector.configure(bootProperties);
    } catch (Exception e) {
    throw new ComponentInitializationException(e);
    }
    Collection locations = collector.collectPluginLocations();
    List<PluginLocation> newLocations = new ArrayList<PluginLocation>();


     ****************/

     class LogOutputStream extends ByteArrayOutputStream {

        LogOutputStream() {
            super();
        }

        public void flush() throws IOException {
            String message = toString();
            if (!LINE_SEP.equals(message) && !"".equals(message)) {
                logger.debug(message);
            }
            reset();
        }
    }
}