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
using MouseKeyboardActivityMonitor.WinApi;
using MouseKeyboardActivityMonitor;
using System.Windows.Forms;
using Utilities;
using WpfAsservissementDisplay;

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

        private readonly KeyboardHookListener m_KeyboardHookManager;

        public MainWindow()
        {
            InitializeComponent();

            serialPort1 = new ReliableSerialPort("COM14", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 20);
            timerAffichage.Tick += TimerAffichage_Tick;
            timerAffichage.Start();

            m_KeyboardHookManager = new KeyboardHookListener(new GlobalHooker());
            m_KeyboardHookManager.Enabled = true;
            m_KeyboardHookManager.KeyDown += HookManager_KeyDown;
           // m_KeyboardHookManager.KeyUp += HookManager_KeyUp;

        }

        bool autoControlActivated = true;

        private void buttonMode_Click(object sender, RoutedEventArgs e)
        {
            autoControlActivated = !autoControlActivated;

            if(autoControlActivated == true)
            {
                buttonMode.Background = Brushes.Crimson;
                buttonMode.Content = "Automatique"; //auto
                UartEncodeAndSendMessage(0x0052, 1, new byte[] {1});
            }
            else
            {
                buttonMode.Background = Brushes.Bisque;
                buttonMode.Content = "Manuel"; //manu
                UartEncodeAndSendMessage(0x0052, 1, new byte[] {0});
            }

            
        }

        private void HookManager_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
            {

            if (autoControlActivated == false)
            {
                switch (e.KeyCode)
                    {
                    case Keys.Left:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[]{(byte)StateRobot.STATE_TOURNE_SUR_PLACE_GAUCHE});
                        break;

                    case Keys.Right:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[] {(byte)StateRobot.STATE_TOURNE_SUR_PLACE_DROITE});
                        break;

                    case Keys.Up:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[]{(byte)StateRobot.STATE_AVANCE});
                        break;

                    case Keys.Down:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[]{(byte)StateRobot.STATE_ARRET});
                        break;

                    case Keys.PageDown:
                        UartEncodeAndSendMessage(0x0051, 1, new byte[]{(byte)StateRobot.STATE_RECULE});
                        break;
                }
                }
            }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            //if(robot.receivedText != "")
            //{

            //    textBoxRéception.Text += robot.receivedText;
            //    robot.receivedText = "";
            //}

            //while(robot.byteListReceived.Count != 0)
            //{
            //    byte byteReceived = robot.byteListReceived.Dequeue();
            //    DecodeMessage(byteReceived);
            //    //string lecture;
            //    //lecture = byteReceived.ToString("X2");
            //    //textBoxRéception.Text += lecture + " ";
            //}

            while(MessageQueue.Count>0)
            {
                var msg = MessageQueue.Dequeue();
                ProcessDecodedMessage(msg.Function, msg.PayloadLength, msg.Payload);
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
                DecodeMessage(b);
                //robot.byteListReceived.Enqueue(b);
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

        void Led_Orange_checked(object sender, RoutedEventArgs e)
        {
            byte[] chaine = { 0, Convert.ToByte(Led1.IsChecked) };
            UartEncodeAndSendMessage(0x0020, 2, chaine);
        }



        void Led_Orange_unchecked(object sender, RoutedEventArgs e)
        {
            byte[] chaine = { 0, Convert.ToByte(Led1.IsChecked) };
            UartEncodeAndSendMessage(0x0020, 2, chaine);
        }

        void Led_Bleue_checked(object sender, RoutedEventArgs e)
        {
            byte[] chaine = { 1, Convert.ToByte(Led2.IsChecked) };
            UartEncodeAndSendMessage(0x0020, 2, chaine);
        }

        void Led_Bleue_unchecked(object sender, RoutedEventArgs e)
        {
            byte[] chaine = { 1, Convert.ToByte(Led2.IsChecked) };
            UartEncodeAndSendMessage(0x0020, 2, chaine);
        }

        void Led_Blanche_checked(object sender, RoutedEventArgs e)
        {
            byte[] chaine = { 2, Convert.ToByte(Led3.IsChecked) };
            UartEncodeAndSendMessage(0x0020, 2, chaine);
        }

        void Led_Blanche_unchecked(object sender, RoutedEventArgs e)
        {
            byte[] chaine = { 2, Convert.ToByte(Led3.IsChecked) };
            UartEncodeAndSendMessage(0x0020, 2, chaine);
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
            } //Lorsqu'on clique, le bouton devient bleu.
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

            //byte[] chaine = new byte[1];
            //chaine[0] = Convert.ToByte(1);
            // UartEncodeAndSendMessage(0x0052, 1, chaine);

            envoiPID();

            //byte[] chaine2 = new byte[1];
            //chaine2[0] =Convert.ToByte(StateRobot.STATE_AVANCE);

            //UartEncodeAndSendMessage(0x0051, chaine2.Length, chaine2);
        }
        
        
        void envoiPID()
        {
            double Kp = 3;
            double Ki = 70;
            double Kd = 0;
            double KpMax = 1000;
            double KiMax = 1000;
            double KdMax = 1000;
            double KpT = 3;
            double KiT = 70;
            double KdT = 0;
            double KpMaxT = 1000;
            double KiMaxT = 1000;
            double KdMaxT = 1000;

            int pos = 0;

            byte[] chaine = new byte[48];
            var v = BitConverter.GetBytes((float)Kp);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)Ki);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)Kd);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KpMax);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KiMax);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KdMax);
            v.CopyTo(chaine, pos);
            pos += 4;



            v = BitConverter.GetBytes((float)KpT);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KiT);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KdT);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KpMaxT);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KiMaxT);
            v.CopyTo(chaine, pos);
            pos += 4;

            v = BitConverter.GetBytes((float)KdMaxT);
            v.CopyTo(chaine, pos);
            pos += 4;

            UartEncodeAndSendMessage(0x0013, pos, chaine);
        }

        private void textBoxEmission_KeyUp(object sender, System.Windows.Input.KeyEventArgs e)
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
                        //textBoxRéception.Text += "Pas d'erreur \n";
                        EnqueueDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                        //ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        Dispatcher.Invoke(delegate
                        {
                            // Do the dirty work of my method here.
                            textBoxRéception.Text += "Erreur les amis :') =>" + msgDecodedFunction.ToString()+ "\n";
                        });
                        //textBoxRéception.Text += msgDecodedFunction.ToString() + msgDecodedPayloadLength.ToString() + msgDecodedPayload.ToString() + "\n\r";
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
            Vitesse = 0x0040,
            Etape = 0x0050,
            Set_Robot_State = 0x0051,
            Set_Robot_Manual_Control = 0x0052,
            Position_Data = 0x0061,
            Data_asserv = 0x0028
        }

        public enum StateRobot
        {
             STATE_ATTENTE = 0,
             STATE_ATTENTE_EN_COURS = 1,
             STATE_AVANCE = 2,
             STATE_AVANCE_EN_COURS = 3,
             STATE_TOURNE_GAUCHE = 4,
             STATE_TOURNE_GAUCHE_EN_COURS = 5,
             STATE_TOURNE_GAUCHE_PROCHE = 6,
             STATE_TOURNE_GAUCHE_PROCHE_EN_COURS = 7,
             STATE_TOURNE_DROITE = 8,
             STATE_TOURNE_DROITE_EN_COURS = 9,
             STATE_TOURNE_DROITE_PROCHE = 10,
             STATE_TOURNE_DROITE_PROCHE_EN_COURS = 11,
             STATE_TOURNE_SUR_PLACE_GAUCHE = 12,
             STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 13,
             STATE_TOURNE_SUR_PLACE_DROITE = 14,
             STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 15,
             STATE_ARRET = 16,
             STATE_ARRET_EN_COURS = 17,
             STATE_RECULE = 18,
             STATE_RECULE_EN_COURS = 19,
             STATE_TETE_TETE = 20,
             STATE_TETE_TETE_EN_COURS = 21,
             STATE_TOURNE_EX_GAUCHE = 22,
             STATE_TOURNE_EX_GAUCHE_EN_COURS = 23,
             STATE_TOURNE_EX_DROITE = 24,
             STATE_TOURNE_EX_DROITE_EN_COURS = 25,
             STATE_FREINAGE1 = 26,
             STATE_FREINAGE1_EN_COURS = 27,
             STATE_FREINAGE2 = 28,
             STATE_FREINAGE2_EN_COURS = 29,
             STATE_COULOIR_TOURNE_GAUCHE = 30,
             STATE_COULOIR_TOURNE_GAUCHE_EN_COURS = 31,
             STATE_COULOIR_TOURNE_DROITE = 32,
             STATE_COULOIR_TOURNE_DROITE_EN_COURS = 33

        }

        Queue<Message> MessageQueue = new Queue<Message>();
        void EnqueueDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            MessageQueue.Enqueue(new Message(msgFunction, msgPayloadLength, msgPayload));
        }
        
        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte [] msgPayload)
        {
            switch(msgFunction)
            {
                case (int)Fonctions.Texte:
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

                    int ir_ex_gauche = msgPayload[0];
                    int ir_gauche = msgPayload[1];
                    int ir_centre = msgPayload[2];
                    int ir_droit = msgPayload[3];
                    int ir_ex_droit = msgPayload[4];

                    IRExGauche.Text = "IR Ex Gauche : " + ir_ex_gauche + " cm";
                    IRGauche.Text =  "IR Gauche : " + ir_gauche + " cm";
                    IRCentre.Text = "IR Centre : " + ir_centre + " cm";
                    IRDroit.Text = "IR Droit : " + ir_droit + " cm";
                    IRExDroit.Text = "IR Ex Droit : " + ir_ex_droit + " cm";

                    break;

                case (int)Fonctions.Vitesse:
                    int m_gauche = msgPayload[0];
                    int m_droit = msgPayload[1];

                    M_gauche.Text = "Vitesse gauche : " + m_gauche + " %";
                    M_droit.Text = "Vitesse droite : " + m_droit + " %";
                    break;

                case (int)Fonctions.Etape:
                    int etape = msgPayload[0];
                    int t_0 = msgPayload[1];
                    int t_1 = msgPayload[2];
                    int t_2 = msgPayload[3];
                    int t_3 = msgPayload[4];

                    int tps_courant = (t_0 << 24) + (t_1 << 16) + (t_2 << 8) + (t_3);

                    Time.Text = tps_courant.ToString() + "ms";
                    State.Text = "State Robot : " + (StateRobot) etape;
                    break;

                case (int)Fonctions.Set_Robot_State:
                    
                    break;

                case (int)Fonctions.Position_Data:
                    byte[] tab = msgPayload.GetRange(0, 4);
                    robot.timestamp += tab.GetFloat();

                    tab = msgPayload.GetRange(4, 4);
                    robot.positionXOdo = tab.GetFloat();

                    tab = msgPayload.GetRange(8, 4);
                    robot.positionYOdo = tab.GetFloat();

                    //robot.positionXOdo += BitConverter.ToSingle(msgPayload, 0);

                    tab = msgPayload.GetRange(12, 4);
                    robot.angleRadianOdo = tab.GetFloat();

                    tab = msgPayload.GetRange(16, 4);
                    robot.vitesseLineaireOdo = tab.GetFloat();

                    tab = msgPayload.GetRange(20, 4);
                    robot.vitesseAngulaireOdo = tab.GetFloat();

                    timestamp.Text = "Durée prog : " + (robot.timestamp).ToString();
                    Pos_X.Text = "Position x : " + (robot.positionXOdo).ToString();
                    Pos_Y.Text = "Position y : " + (robot.positionYOdo).ToString();
                    Angle_rad.Text = "Angle rad : " + (robot.angleRadianOdo * 180 / Math.PI).ToString();
                    Linear_speed.Text = "Vitesse linéaire : " + (robot.vitesseLineaireOdo).ToString();
                    Angular_speed.Text = "Vitesse angulaire : " + (robot.vitesseAngulaireOdo).ToString();
                    break;

                case (int)Fonctions.Data_asserv:
                    int pos = 0;

                    double consigneX;
                    double consigneTheta;
                    double mesureX;
                    double mesureTheta;
                    double errorX;
                    double errorTheta;
                    double commandX;
                    double commandTheta;
                    //-------------------
                    double corrPX;
                    double corrPTheta;
                    double corrIX;
                    double corrITheta;
                    double corrDX;
                    double corrDTheta;
                    //-------------------
                    double KpX;
                    double KpTheta;
                    double KiX;
                    double KiTheta;
                    double KdX;
                    double KdTheta;
                    //-------------------
                    double corrMaxPX;
                    double corrMaxPTheta;
                    double corrMaxIX;
                    double corrMaxITheta;
                    double corrMaxDX;
                    double corrMaxDTheta;
                    //-------------------
                    double vitesseGauche;
                    double vitesseDroite;



                    consigneX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;
                    
                    consigneTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    mesureX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;
                    
                    mesureTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;
                    
                    errorX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;
                    
                    errorTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;
                    
                    commandX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;
                    
                    commandTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    
                    corrPX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrPTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrIX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrITheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrDX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrDTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;


                    KpX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    KpTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    KiX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    KiTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    KdX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    KdTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;


                    corrMaxPX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrMaxPTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrMaxIX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrMaxITheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrMaxDX = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    corrMaxDTheta = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    vitesseGauche = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    vitesseDroite = BitConverter.ToSingle(msgPayload, pos);
                    pos += 4;

                    asservSpeedDisplay.UpdatePolarSpeedConsigneValues(consigneX, consigneTheta);
                    asservSpeedDisplay.UpdatePolarSpeedCommandValues(commandX, commandTheta);
                    asservSpeedDisplay.UpdatePolarSpeedErrorValues(errorX,errorTheta);
                    asservSpeedDisplay.UpdatePolarSpeedCorrectionValues(corrPX, corrPTheta, corrIX, corrITheta, corrDX, corrDTheta);
                    asservSpeedDisplay.UpdatePolarSpeedCorrectionGains(KpX, KpTheta, KiX, KiTheta, KdX, KdTheta);
                    asservSpeedDisplay.UpdatePolarSpeedCorrectionLimits(corrMaxPX, corrMaxPTheta, corrMaxIX, corrMaxITheta, corrMaxDX, corrMaxDTheta);
                    //asservSpeedDisplay.UpdatePolarOdometrySpeed(mesureX, mesureTheta);
                    asservSpeedDisplay.UpdatePolarOdometrySpeed(robot.vitesseLineaireOdo, robot.vitesseAngulaireOdo);
                    asservSpeedDisplay.UpdateIndependantOdometrySpeed(vitesseGauche, vitesseDroite);

                    break;
            }
        }
    }

    public class Message 
    {
        public int Function;
        public int PayloadLength;
        public byte[] Payload;

        public Message(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            Function = msgFunction;
            PayloadLength = msgPayloadLength;
            Payload = msgPayload;
        }
    }
}
