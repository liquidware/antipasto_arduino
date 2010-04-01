package antipasto.Plugins.Interfaces;


import java.util.EventListener;
import java.util.EventObject;

public interface EditorListener extends EventListener
{
   void handleEvent(EventObject event);
} 