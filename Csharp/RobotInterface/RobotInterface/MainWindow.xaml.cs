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
            serialPort1 = new ReliableSerialPort("COM18", 115200, Parity.None, 8, StopBits.One);
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
            //if(robot.receivedText != "")
            //{

            //    textBoxRéception.Text += robot.receivedText;
            //    robot.receivedText = "";
            //}

            while(robot.byteListReceived.Count != 0)
            {
                byte byteReceived = robot.byteListReceived.Dequeue();
                DecodeMessage(byteReceived);
                //string lecture;
                //lecture = byteReceived.ToString("X2");
                //textBoxRéception.Text += lecture + " ";
            }
        }


        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //for(int i=0; i<e.Data.Length; i++)
            //{
            //    robot.byteListReceived.Enqueue(e.Data[i]);
            //}

            foreach(byte b in e.Data)
            {
                robot.byteListReceived.Enqueue(b);
            }

            //robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);            
        }

        //public void DecodeMessageReceived(object sender, DataReceivedArgs e)
        //{
        //    foreach(byte b in e.Data)
        //    {
        //        DecodeMessage(b);
        //    }
        //}

        void SendMessage()
        {
            string message = textBoxEmission.Text.TrimEnd('\n' );
            serialPort1.WriteLine(message);
            byte[] chaine = Encoding.ASCII.GetBytes(message);
            UartEncodeAndSendMessage(0x0080, chaine.Length, chaine);
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

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            //byte[] byteList = new byte[20];
            //for (int i = 0; i < 20; i++)
            //{
            //    byteList[i] = (byte)(2*i);

            //}
            //serialPort1.Write(byteList, 0, 20);
            //byte[] chaine = Encoding.ASCII.GetBytes("Bonjour");
            byte[] chaine = new byte[3];
            chaine[0] = Convert.ToByte(52);
            chaine[1] = Convert.ToByte(14);
            UartEncodeAndSendMessage(0x0040, chaine.Length, chaine);
        }


        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
        byte CalculateChecksum(int msgFunction,int msgPayloadLength, byte [] msgPayload )
        {
            byte Checksum;
            Checksum = 0xFE;
            Checksum ^= (byte)(msgFunction >> 8);
            Checksum ^= (byte)(msgFunction >> 0);
            Checksum ^= (byte)(msgPayloadLength >> 8);
            Checksum ^= (byte)(msgPayloadLength >> 0);
            for(int i=0 ; i < msgPayloadLength ; i++)
            {
                Checksum ^= msgPayload[i];
            }
            return Checksum;  
        }

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[ ] msgPayload)
        {
            byte checksum = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            byte[] msg = new byte[6 + msgPayloadLength];
            int pos = 0;
            msg[pos++] = 0xFE;
            msg[pos++] = (byte)(msgFunction >> 8);
            msg[pos++] = (byte)(msgFunction >> 0);
            msg[pos++] = (byte)(msgPayloadLength >> 8);
            msg[pos++] = (byte)(msgPayloadLength >> 0);
            for(int i=0; i<msgPayloadLength; i++)
            {
                msg[pos++] = msgPayload[i];
            }
            msg[pos++] = checksum;
            serialPort1.Write(msg, 0, 6+msgPayloadLength);
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if( c == 0xFE)
                    {
                        rcvState = StateReception.FunctionMSB;
                        msgDecodedFunction = 0;
                    }
                break;

                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                break;

                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
                break;

                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;

                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;
                    if(msgDecodedPayloadLength == 0)
                        rcvState = StateReception.CheckSum;
                    else if(msgDecodedPayloadLength>=1024)
                        rcvState = StateReception.Waiting;
                    else
                    {
                        rcvState = StateReception.Payload;
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                    }
                    break;

                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                    if(msgDecodedPayloadIndex>=msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                break;

                case StateReception.CheckSum:
                    byte calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    byte receivedChecksum = c;
                    if (calculatedChecksum == receivedChecksum)
                    {
                        //Success, on a un message valide
                        textBoxRéception.Text += "Pas d'erreur \n";
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        textBoxRéception.Text += "Erreur les amis :')" + "\n";
                    }
                    rcvState = StateReception.Waiting;

                    break;

                default:
                    rcvState = StateReception.Waiting;
                break;
            }
        }

        public enum Fonctions
        {
            Texte = 0x0080,
            LED = 0x0020,
            Tel_IR = 0x0030,
            Vitesse = 0x0040
        }

        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte [] msgPayload)
        {
            switch(msgFunction)
            {
                case (int) Fonctions.Texte:
                    textBoxRéception.Text += Encoding.UTF8.GetString(msgPayload, 0, msgPayload.Length) + "\n";
                    break;

                case (int)Fonctions.LED:

                    int numLED = msgPayload[0];
                    int etatLED = msgPayload[1];

                    if (numLED == 0)
                    {
                        Led1.IsChecked = Convert.ToBoolean(etatLED);
                    }

                    else if (numLED == 1)
                    {
                        Led2.IsChecked = Convert.ToBoolean(etatLED);
                    }

                    else if (numLED == 2)
                    {
                        Led3.IsChecked = Convert.ToBoolean(etatLED);
                    }
                    break;

                case (int)Fonctions.Tel_IR:

                    int ir_gauche = msgPayload[0];
                    int ir_centre = msgPayload[1];
                    int ir_droit = msgPayload[2];
                     
                    IRGauche.Text =  "IR Gauche : " + ir_gauche + " cm";
                    IRCentre.Text = "IR Centre : " + ir_centre + " cm";
                    IRDroit.Text = "IR Droit : " + ir_droit + " cm";

                    break;

                case (int)Fonctions.Vitesse:
                    int m_gauche = msgPayload[0];
                    int m_droit = msgPayload[1];

                    M_gauche.Text = "Vitesse gauche : " + m_gauche + " %";
                    M_droit.Text = "Vitesse droite : " + m_droit + " %";
                    break;
            }
        }
    }
}
