using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace cBluetooth
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static string comPoort = "";
        private static SerialPort serialPort = new SerialPort();
        public MainWindow()
        {
            InitializeComponent();
            
            Loaded += MainWindow_Loaded;
            cboCompoort.SelectionChanged += CboCompoort_SelectionChanged;
            btnVooruit.Click += BtnVooruit_Click;
            btnAchteruit.Click += BtnAchteruit_Click;
            btnLinks.Click += BtnLinks_Click;
            btnRechts.Click += BtnRechts_Click;
            rbAuto.Click += RbAuto_Click;
            rbManueel.Click += RbManueel_Click;
        }

        private void CboCompoort_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            comPoort = cboCompoort.SelectedItem.ToString();
            serialPort.PortName = comPoort;
        }

        private void RbManueel_Click(object sender, RoutedEventArgs e)
        {
            serialPort.Open();
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine("4");
            }
            serialPort.Close();
        }

        private void RbAuto_Click(object sender, RoutedEventArgs e)
        {
            serialPort.Open();
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine("5");
            }
            serialPort.Close();
        }

        private void BtnRechts_Click(object sender, RoutedEventArgs e)
        {
           
            serialPort.Open();
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine("3");
            }
            serialPort.Close();
        }

        private void BtnLinks_Click(object sender, RoutedEventArgs e)
        {
            serialPort.Open();
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine("2");
            }
            serialPort.Close();
        }

        private void BtnAchteruit_Click(object sender, RoutedEventArgs e)
        {
            serialPort.Open();
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine("1");
            }
            serialPort.Close();
        }

        private void BtnVooruit_Click(object sender, RoutedEventArgs e)
        {
            // throw new NotImplementedException();
            serialPort.Open();
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine("0");
            }
            serialPort.Close();
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            string[] comPoorten = SerialPort.GetPortNames();
            cboCompoort.Items.Clear();
            foreach(string poort in comPoorten)
            {
                cboCompoort.Items.Add(poort);
                
            }
                cboCompoort.Items.Insert(0, "Selecteer een poort");
               cboCompoort.SelectedIndex = 0;
            serialPort.BaudRate = 9600;
            serialPort.PortName = "COM20"; 
            }
    }
}
