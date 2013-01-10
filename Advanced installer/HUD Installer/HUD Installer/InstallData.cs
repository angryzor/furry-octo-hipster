using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

namespace HUD_Installer
{
    public class InstallData
    {
        public InstallData()
        {
            data_ = XDocument.Load("installdata.xml");
        }

        public static IEnumerable<XElement> GetAllInCategory(string cat)
        {
            return self.data_.Element(InstallData.xmlns + "InstallData").Elements(InstallData.xmlns + "Component").Where(delegate(XElement el)
            {
                foreach (XElement ecat in el.Elements(InstallData.xmlns + "Category"))
                {
                    if(MatchCategory(ecat.Value,cat))
                        return true;
                }
                return false;
            });
        }

        public static XElement GetComponent(string id)
        {
            return (from comp in self.data_.Element(InstallData.xmlns + "InstallData").Elements(InstallData.xmlns + "Component")
                    where comp.Element(InstallData.xmlns + "Id").Value == id
                    select comp).First();
        }

        public static bool MatchCategory(string matchee, string matcher)
        {
            return  matchee.Length >= matcher.Length
                &&  matchee.Substring(0, matcher.Length) == matcher
                &&  (   matchee.Length == matcher.Length 
                     || matchee[matcher.Length] == '/'      );
        }

        public static bool AreConstraintsMet(string comp)
        {
            return true;
        }

        public static XNamespace xmlns = "http://www.fpsbanana.com/guis/23833/installdata";

        XDocument data_;
        static InstallData self = new InstallData();
    }
}
