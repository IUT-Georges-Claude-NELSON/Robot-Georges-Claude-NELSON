using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
using ExtendedSerialPort;
using System.IO.Ports;
using System.Windows.Threading;


namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    /// 
   
    public partial class MainWindow : Window
    {

        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;

        Robot robot = new Robot();


        public MainWindow()
        {
            serialPort1 = new ReliableSerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            InitializeComponent();
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            if(robot.receivedText != "")
            {
                textBoxRéception.Text += robot.receivedText;
                robot.receivedText = "";
            }
        }


        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        }

        void SendMessage()
        {
            string message = textBoxEmission.Text.TrimEnd('\n' );
            serialPort1.WriteLine(message);
            textBoxEmission.Text = "";
        }

        bool toggle = false;

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (toggle)
            { 
                buttonEnvoyer.Background = Brushes.Beige; 
            }
            else
            { 
                buttonEnvoyer.Background = Brushes.RoyalBlue; 
            } //Lorsqu'on clique, le bouton devient bleu. Si l'on réitère, le bouton reste toujours bleu.
            SendMessage();

            toggle = !toggle;
        }

        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxRéception.Text = "";
        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
    }
}
