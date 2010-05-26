using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Linq;

namespace HUD_Installer
{
    class ICComboBoxItem : ComboBoxItem, Updateable
    {
        public ICComboBoxItem(XElement comp)
        {
            id = comp.Element(InstallData.xmlns + "Id").Value;

            this.Content = new InstallChoiceItem(comp);
        }

        public void InstallUpdate()
        {
            if (InstallData.AreConstraintsMet(id)) 
                this.IsEnabled = true;
            else 
                this.IsEnabled = false;
        }

        public string GetId()
        {
            return id;
        }

        string id;
    }

    /// <summary>
    /// Interaction logic for InstallChoice.xaml
    /// </summary>
    public partial class InstallChoice : UserControl, Updateable, Selectable
    {
        public InstallChoice(XElement choice)
        {
            InitializeComponent();

            choicelabel.Content = choice.Element(MainWindow.xmlns_layout + "Label").Value;

            foreach (XElement el in choice.Elements(MainWindow.xmlns_layout + "Category"))
            {
                foreach (XElement comp in InstallData.GetAllInCategory(el.Value))
                {
                    ICComboBoxItem cbi = new ICComboBoxItem(comp);

                    choices.Items.Add(cbi);
                }
            }

            foreach (XElement el in choice.Elements(MainWindow.xmlns_layout + "Component"))
            {
                ICComboBoxItem cbi = new ICComboBoxItem(InstallData.GetComponent(el.Value));

                choices.Items.Add(cbi);
            }
        }

        public void InstallUpdate()
        {
            foreach (Updateable cbi in choices.Items)
            {
                cbi.InstallUpdate();
            }
        }

        public bool IsSelected(string id)
        {
            ICComboBoxItem cbi = (ICComboBoxItem)choices.SelectedItem;
            if (cbi == null)
                return false;

            return id == cbi.GetId();
        }
    }
}
