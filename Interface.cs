using System;
using Gtk;
using System.Diagnostics;

public class InterfaceClient : Window
{
    private Button _btnEnvoyer;

    public InterfaceClient() : base("Interface Client")
    {
        SetDefaultSize(300, 200);
        SetPosition(WindowPosition.Center);

        // Création du bouton
        _btnEnvoyer = new Button("Envoyer");
        _btnEnvoyer.Clicked += OnEnvoyerClicked;

        // Ajouter le bouton à la fenêtre
        Box vbox = new Box(Orientation.Vertical, 0); // Utilisation de Box au lieu de VBox
        vbox.PackStart(_btnEnvoyer, false, false, 0);
        Add(vbox);

        // Lier l'événement de fermeture de la fenêtre
        DeleteEvent += (sender, e) => OnWindowClosed();

        ShowAll();
    }

    private void OnEnvoyerClicked(object sender, EventArgs e)
    {
        // Quand l'utilisateur clique sur le bouton "Envoyer", appel à la méthode du client C
        Process.Start("chemin/vers/client.exe", "message à envoyer");
    }

    private void OnWindowClosed()
    {
        // Quand la fenêtre se ferme, on appelle le client pour déconnecter proprement le client
        Process.Start("chemin/vers/client.exe", "DECONNEXION");
        Application.Quit();  // Quitte l'application GTK
    }

    public static void Main()
    {
        Application.Init();
        new InterfaceClient();
        Application.Run();
    }
}
