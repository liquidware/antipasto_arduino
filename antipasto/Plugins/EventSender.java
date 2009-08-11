
package antipasto.Plugins;

import java.util.EventObject;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import antipasto.Plugins.Interfaces.*;


public class EventSender
{
   EventSender()
   {
   }
   
   public void broadcast(EventObject event)
   {
      //FIXME: needs to be much more efficient
      for (Iterator i = listeners.iterator(); i.hasNext(); )
      {
    	 System.out.println("DISPATCHING EVENT TO:"+ this.toString());
         ListenerEntry l = (ListenerEntry) i.next();
         //if (l.getEntryClass().isAssignableFrom(event.getClass()))
         //{        	 
            l.getListener().handleEvent(event);
         //}
      }
   }
   public boolean hasListeners(Class c)
   {
      // FIXME:
      return true;
   }
   public void addEventListener(EditorListener l, Class c)
   {
      listeners.add(new ListenerEntry(l,c));
   }
   public void removeEventListener(EditorListener l, Class c)
   {
      listeners.remove(new ListenerEntry(l,c));
   }
   private Set listeners = new HashSet();
   private class ListenerEntry
   {
      private EditorListener l;
      private Class c;
      ListenerEntry(EditorListener l, Class c)
      {
         this.l = l;
         this.c = c;
      }
      public boolean equals(Object o)
      {
         if (o instanceof ListenerEntry)
         {
            ListenerEntry that = (ListenerEntry) o;
            return this.l == that.l && this.c == that.c; 
         }
         else return false;
      }
      public int hashCode()
      {
         return l.hashCode() + c.hashCode();
      }
      Class getEntryClass()
      {
         return c;
      }
      EditorListener getListener()
      {
         return l;
      }
   }
}