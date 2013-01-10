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
    /// <summary>
    /// Interaction logic for InstallCheckbox.xaml
    /// </summary>
    public partial class InstallCheckbox : UserControl, Updateable, Selectable
    {
        public InstallCheckbox(XElement cb)
        {
            InitializeComponent();

            id = cb.Element(MainWindow.xmlns_layout + "Id").Value;
            check.Content = InstallData.GetComponent(id).Element(InstallData.xmlns + "Name").Value;

            BitmapImage bi = new BitmapImage();
            bi.BeginInit();
            bi.UriSource = new Uri(@"pictures\" + InstallData.GetComponent(id).Element(InstallData.xmlns + "Image").Value, UriKind.Relative);
            bi.EndInit();

            image.Source = bi;
        }

        public void InstallUpdate()
        {
            if(InstallData.AreConstraintsMet(id))
                check.IsEnabled = true;
            else
                check.IsEnabled = false;
        }

        public bool IsSelected(string id)
        {
            return this.id == id && check.IsChecked == true;
        }

        string id;
    }
}
