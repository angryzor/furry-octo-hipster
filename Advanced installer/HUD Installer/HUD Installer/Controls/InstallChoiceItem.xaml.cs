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
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class InstallChoiceItem : UserControl
    {
        public InstallChoiceItem(XElement comp)
        {
            InitializeComponent();

            label.Content = comp.Element(InstallData.xmlns + "Name").Value;
            BitmapImage bi = new BitmapImage();
            bi.BeginInit();
            bi.UriSource = new Uri(@"pictures\" + comp.Element(InstallData.xmlns + "Image").Value, UriKind.Relative);
            bi.EndInit();

            image.Source = bi;
        }
    }
}
