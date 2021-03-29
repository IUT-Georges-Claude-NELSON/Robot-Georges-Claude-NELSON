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

namespace RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        
        public MainWindow()
        {
            InitializeComponent();
        }

        void SendMessage()
        {
            string message = textBoxEmission.Text.TrimEnd('\n');
            textBoxRéception.Text = textBoxRéception.Text + "Reçu : " + message + "\n";
            textBoxEmission.Text = "";
        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
            { 
                buttonEnvoyer.Background = Brushes.Beige; 
            }
            else
            { 
                buttonEnvoyer.Background = Brushes.RoyalBlue; 
            } //Lorsqu'on clique, le bouton devient bleu. Si l'on réitère, le bouton reste toujours bleu.
            SendMessage();
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
