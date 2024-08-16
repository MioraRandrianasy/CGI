#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string> // Pour utiliser std::string

int main() {
    std::cout << "Content-Type: text/html\n\n";

    char* requette = std::getenv("QUERY_STRING");
    if (!requette) {
        std::cout << "<p>Aucune donnée</p>";
        return EXIT_SUCCESS;
    }

    char* login = std::strstr(requette, "login=");
    char* password = std::strstr(requette, "password=");

    if (!login || !password) {
        std::cout << "<p>Données de formulaire incomplètes.</p>";
        return EXIT_SUCCESS;
    }

    // Avancement des pointeurs pour ignorer les noms des paramètres
    login += std::strlen("login=");
    password += std::strlen("password=");

    std::string login_str(login);
    std::size_t pos = login_str.find("&password=");
    if (pos != std::string::npos) {
        login_str.erase(pos);
    }

    // Lecture du fichier database.txt
    std::ifstream database("dataLog.txt");
    if (!database.is_open()) {
        std::cout << "<p>Erreur lors de l'ouverture de la base de données.</p>";
        return EXIT_FAILURE;
    }

    std::string db_login, db_password;
    bool found = false;
    while (database >> db_login >> db_password) {
        if (db_login == login_str && db_password == password) {
            found = true;
            break;
        }
    }
    database.close();

    // Affichage des résultats
    std::cout << "<html><body>";
    if (found) {
        std::cout << "<p>Connexion réussie pour l'utilisateur : " << login_str << "</p>";
    } else {
        std::cout << "<p>Échec de la connexion. Identifiant ou mot de passe incorrect.</p>";
        std::cout << R"(
            <form method="get" action="">
                <label for="login">Login :</label><br>
                <input type="text" id="login" name="login"><br>
                <label for="pass">Password :</label><br>
                <input type="password" id="pass" name="pass"><br><br>
                <input type="submit" value="Se connecter">
            </form>
        )";
    }
    std::cout << "</body></html>";

    return EXIT_SUCCESS;
}
