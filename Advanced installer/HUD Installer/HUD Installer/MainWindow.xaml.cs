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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            layout_ = XDocument.Load("installlayout.xml");
            presets_ = XDocument.Load("installpresets.xml");

            PopulateTabs();
        }

        protected void PopulateTabs()
        {
            foreach (XElement tab in layout_.Element(xmlns_layout + "InstallLayout").Elements(xmlns_layout + "Tab"))
            {
                AddTab(tab);
            }
        }

        protected void AddTab(XElement tab)
        {
            TabItem ti = new TabItem();

            ti.Header = tab.Element(xmlns_layout + "Title").Value;

            StackPanel sp = new StackPanel();
            ScrollViewer sv = new ScrollViewer();

            sv.Content = sp;
            sv.CanContentScroll = true;

            ti.Content = sv;
            
            IEnumerable<XElement> els = from el in tab.Elements()
                                        where el.Name == (xmlns_layout + "Choice") || el.Name == (xmlns_layout + "Checkbox")
                                        select el;

            foreach (XElement el in els)
            {
                AddOption(sp,el);
            }

            tabs.Items.Add(ti);
        }

        protected void AddOption(StackPanel ti, XElement el)
        {
            if (el.Name == (xmlns_layout + "Choice")) ti.Children.Add(new InstallChoice(el));
            else ti.Children.Add(new InstallCheckbox(el));
        }

        public void InstallUpdate()
        {
            foreach (TabItem tab in tabs.Items)
            {
                foreach (Updateable c in ((StackPanel)((ScrollViewer)tab.Content).Content).Children)
                {
                    c.InstallUpdate();
                }
            }
        }

        public static XNamespace xmlns_layout = "http://www.fpsbanana.com/guis/23833/installlayout";

        private XDocument layout_;
        private XDocument presets_;
    }
}
