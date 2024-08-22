#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void connaitre_classe_ip(int octet1, char *classe) {
    if (octet1 <= 127) {
        classe[0] = 'A';
    } else if (octet1 <= 191) {
        classe[0] = 'B';
    } else if (octet1 <= 223) {
        classe[0] = 'C';
    } else if (octet1 <= 239) {
        classe[0] = 'D';
    } else {
        classe[0] = 'E';
    }
    classe[1] = '\0';  // Terminer la chaîne de caractères
}

int extraire_octets(char *adresse_ip, int *octet1, int *octet2, int *octet3, int *octet4) {
    return sscanf(adresse_ip, "%d.%d.%d.%d", octet1, octet2, octet3, octet4);
}

int verifier_adresse_ip(int octet1, int octet2, int octet3, int octet4) {
    return (octet1 >= 0 && octet1 <= 255 &&
            octet2 >= 0 && octet2 <= 255 &&
            octet3 >= 0 && octet3 <= 255 &&
            octet4 >= 0 && octet4 <= 255);
}

int main() {
    char *query_string = getenv("QUERY_STRING");
    if (query_string == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h1>Erreur</h1><p>Pas de données reçues.</p></body></html>");
        return 1;
    }

    char adresse_ip[16];
    int octet1, octet2, octet3, octet4;
    char classe[2];

    // Extraire l'adresse IP du query string
    sscanf(query_string, "adresse-ip=%15s", adresse_ip);

    // Décoder les caractères spéciaux
    char decoded_ip[16];
    int j = 0;
    for (int i = 0; adresse_ip[i] != '\0'; i++) {
        if (adresse_ip[i] == '%') {
            int value;
            sscanf(&adresse_ip[i + 1], "%2x", &value);
            decoded_ip[j++] = (char)value;
            i += 2;
        } else {
            decoded_ip[j++] = adresse_ip[i];
        }
    }
    decoded_ip[j] = '\0';

    // Extraire les octets
    int resultat = extraire_octets(decoded_ip, &octet1, &octet2, &octet3, &octet4);

    // Vérifier l'adresse IP
    int valide = (resultat == 4) && verifier_adresse_ip(octet1, octet2, octet3, octet4);

    // Afficher le résultat
    printf("Content-Type: text/html\n\n");
    printf("<html><head><style>"
           "body { font-family: Arial, sans-serif; margin: 20px; padding: 20px; background-color: #f4f4f4; }"
           ".container { max-width: 600px; margin: 0 auto; padding: 20px; background-color: #fff; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }"
           "h1 { text-align: center; }"
           "p { font-size: 18px; }"
           ".error { color: red; }"
           ".success { color: green; }"
           "</style></head><body>");
    printf("<div class='container'>");
    if (valide) {
        connaitre_classe_ip(octet1, classe);
        printf("<h1>Classe de l'adresse IP</h1>");
        printf("<p class='success'>Votre adresse IP %s est de classe %s.</p>", decoded_ip, classe);
    } else {
        printf("<h1>Erreur</h1>");
        printf("<p class='error'>Votre adresse IP est incorrecte.</p>");
    }
    printf("</div></body></html>");

    return 0;
}
