#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Necesaria para fecha y hora

// Definimos el tamaño máximo del catálogo de productos
#define PRO_MAX 500
#define AFI_MAX 150
#define ARCHIVO_INV "inventario.txt"

// Estructura para los productos del inventario
struct Producto {
    int id;
    char nombre[50];
    float precio;
    int stock;
    int tipoIVA;
};

struct Afiliado {
    int id;
    int DNI;
    char nombre[50];
};

//Funcion para limpiar pantalla
void limpiarPantalla() {
    for (int i = 0; i < 100; i++) {
        printf("\n");
    }
}

/*
void ordenarInventarioPorId(struct Producto inventario[], int total) {
    struct Producto temporal;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (inventario[j].id > inventario[j+1].id) {
                temporal = inventario[j];
                inventario[j] = inventario[j+1];
                inventario[j+1] = temporal;
            }
        }
    }
}
*/

// Algoritmo de Búsqueda Binaria
int buscarID(struct Producto inventario[], int total, int idBuscado) {
    int inicio = 0, fin = total - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (inventario[medio].id == idBuscado) return medio;
        if (inventario[medio].id < idBuscado) inicio = medio + 1;
        else fin = medio - 1;
    }
    return -1; 
}

// Funcion para guardar el inventario en un archivo
void guardarInventario(struct Producto inventario[], int totalProductos) {
    FILE *f = fopen(ARCHIVO_INV, "w");
    if (f == NULL) {
        printf("Error: No se pudo guardar el inventario.\n");
        return;
    }
    for (int i = 0; i < totalProductos; i++) {
        fprintf(f, "%d;%s;%.2f;%d;%d\n", inventario[i].id, inventario[i].nombre, inventario[i].precio, inventario[i].stock, inventario[i].tipoIVA);
    }
    fclose(f);
}

// Funcion para cargar el inventario desde un archivo
int cargarInventario(struct Producto inventario[]) {
    FILE *f = fopen(ARCHIVO_INV, "r");
    int contador = 0;
    if (f == NULL) {
        printf("[!] No se encontro inventario previo. Iniciando sistema vacio.\n");
        return 0; 
    }
    while (fscanf(f, "%d;%[^;];%f;%d;%d\n", &inventario[contador].id, inventario[contador].nombre, &inventario[contador].precio, &inventario[contador].stock, &inventario[contador].tipoIVA) == 5) {
        contador++;
        if (contador >= PRO_MAX) break;
    }
    fclose(f);
    return contador;
}

// Funcion para cargar a los afiliados desde un archivo
int cargarAfiliados(struct Afiliado Afiliado[]) {
    FILE *f = fopen("afiliado.txt", "r");
    int contador = 0;
    if (f == NULL) {
        printf("[!] No se encontro el archivo de afiliados. Iniciando sin datos de afiliados.\n");
        return 0;
    }
    while (fscanf(f, "%d;%[^;];%d\n", &Afiliado[contador].id, Afiliado[contador].nombre, &Afiliado[contador].DNI) == 3) {
        contador++;
        if (contador >= AFI_MAX) break;
    }
    fclose(f);
    return contador;
}

void ordenarAfiliados(struct Afiliado Afiliado[], int total) {
    struct Afiliado temporal;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (Afiliado[j].DNI > Afiliado[j+1].DNI) {
                temporal = Afiliado[j];
                Afiliado[j] = Afiliado[j+1];
                Afiliado[j+1] = temporal;
            }
        }
    }
}

int buscarAfiliado(struct Afiliado Afiliado[], int total, int DNIBuscado) {
  int inicio = 0, fin = total - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        if (Afiliado[medio].DNI == DNIBuscado) return medio;
        if (Afiliado[medio].DNI < DNIBuscado) inicio = medio + 1;
        else fin = medio - 1;
    }
    return -1; 
} 

//Funcion para registrar cada venta en el cierre de caja
void registrarVentaEnCierre(int indices[], int cantidades[], int itemsTicket, struct Producto inventario[]) {
    FILE *f = fopen("cierre_de_caja.txt", "a");
    if (f == NULL) return;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(f, "[VENTA %02d:%02d] ", tm.tm_hour, tm.tm_min);
    
    for (int j = 0; j < itemsTicket; j++) {
        int idx = indices[j];
        fprintf(f, "%dx %s (IVA %d) | ", cantidades[j], inventario[idx].nombre, inventario[idx].tipoIVA);
    }
    fprintf(f, "\n"); 
    fclose(f);
}

//Funcion para guardar el cierre de caja en un archivo
void guardarCierreArchivo(float totalCaja, float ventasPorIVA[], float tasasIVA[]) {
    FILE *f = fopen("cierre_de_caja.txt", "a"); // 
    if (f == NULL) {
        printf("Error al crear el archivo de cierre.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(f, "\n_______________________________________\n");
    fprintf(f, "         REPORTE DE CIERRE DE CAJA         \n");
    fprintf(f, "Fecha: %02d/%02d/%d %02d:%02d:%02d\n", 
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, 
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(f, "________________________________________\n");
    
    float totalVentasNetas = 0;
    float totalImpuestos = 0;

    fprintf(f, "%-15s | %-12s | %-10s\n", "Tipo IVA", "Neto", "Impuesto");
    fprintf(f, "----------------------------------------\n");

    for (int i = 1; i <= 3; i++) {
        float neto = ventasPorIVA[i];
        float impuesto = neto * tasasIVA[i];
        
        fprintf(f, "IVA %3.0f%%       | $%10.2f | $%8.2f\n", 
                tasasIVA[i] * 100, neto, impuesto);
        
        totalVentasNetas += neto;
        totalImpuestos += impuesto;
    }

    fprintf(f, "----------------------------------------\n");
    fprintf(f, "SUBTOTAL NETO:           $%15.2f\n", totalVentasNetas);
    fprintf(f, "TOTAL IMPUESTOS:         $%15.2f\n", totalImpuestos);
    fprintf(f, "_______________________________________\n");
    fprintf(f, "TOTAL EFECTIVO EN CAJA:  $%15.2f\n", totalCaja);
    fprintf(f, "_______________________________________\n\n");

    fclose(f);
    printf("\n[i] El cierre ha sido respaldado en 'cierre_de_caja.txt'\n");
}

//Funcion para generar el cierre de caja en pantalla
void generarCierreCaja(float totalCaja, float ventasPorIVA[], float tasasIVA[]) {
    printf("\n_______________________________________\n");
    printf("       Vista previa del cierre         \n");
    printf("_______________________________________\n");
    
    float totalVentasNetas = 0;
    float totalImpuestos = 0;

    printf("%-15s | %-12s | %-10s\n", "Tipo IVA", "Neto", "Impuesto");
    printf("_______________________________________\n");

    for (int i = 1; i <= 3; i++) {
        float neto = ventasPorIVA[i];
        float impuesto = neto * tasasIVA[i];
        
        printf("IVA %3.0f%%       | $%10.2f | $%8.2f\n", 
                tasasIVA[i] * 100, neto, impuesto);
        
        totalVentasNetas += neto;
        totalImpuestos += impuesto;
    }
    printf("_______________________________________\n");
    printf("TOTAL EN CAJA:           $%15.2f\n", totalCaja);
    printf("_______________________________________\n");
}

//Funcion para generar el archivo del ticket
void generarArchivoTicket(int indices[], int cantidades[], int itemsTicket, struct Producto inventario[], float tasasIVA[], float total, float subtotal, float iva) {
    FILE *f = fopen("ticket_abyys_pos.txt", "w");
    if (f == NULL) {
        printf("Error al crear el archivo del ticket.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(f, "_________________________________________\n");
    fprintf(f, "              ABY'S POS                \n");
    fprintf(f, "_________________________________________\n");
    fprintf(f, "FECHA: %02d/%02d/%d  HORA: %02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    fprintf(f, "_________________________________________\n");
    fprintf(f, "%-4s %-20s %-8s %-5s\n", "CANT", "PRODUCTO", "TOTAL", "IVA");
    fprintf(f, "_________________________________________\n");

    for (int j = 0; j < itemsTicket; j++) {
        int idx = indices[j];
        int cant = cantidades[j];
        float precioTotalItem = inventario[idx].precio * cant;
        float tasa = tasasIVA[inventario[idx].tipoIVA] * 100;

        fprintf(f, "%-4d %-20s $%7.2f %3.0f%%\n", cant, inventario[idx].nombre, precioTotalItem, tasa);
    }

    fprintf(f, "_________________________________________\n");
    fprintf(f, "SUBTOTAL NETO:         $%15.2f\n", subtotal);
    fprintf(f, "IVA TOTAL:             $%15.2f\n", iva);
    fprintf(f, "_________________________________________\n");
    fprintf(f, "TOTAL A PAGAR:         $%15.2f\n", total);
    fprintf(f, "_________________________________________\n");
    fprintf(f, "         GRACIAS POR SU COMPRA 🛒        \n");

    fclose(f);
    printf("\n[i] Ticket generado con FECHA y HORA: 'ticket_abyys_pos.txt'\n");
}

// Función principal
int main(void) {
    // Variables generales
    int opcion = 0;
    int subOpcion = 0;
    int i = 0;
    int encontrado = 0;
    int continuar = 0;
    int DNICliente = 0;
    
    // Variables de caja
    int cajaAbierta = 0;
    float totalCaja = 0.0;
    
    // Variables de producto
    int idBuscado, cantidad;
    
    // Variables de venta
    char seguirVendiendo = 's';
    float totalTicket = 0.0;
    float subtotalTicket = 0.0;
    float ivaTicket = 0.0;
    float subtotalItem, ivaItem;
    
    // Variables para el ticket
    int ticketProdIndex[50];
    int ticketCantidad[50];
    int itemsTicket = 0;

    // Arreglo de inventario
    struct Producto inventario[PRO_MAX];
    struct Afiliado Afiliado[AFI_MAX];
    
    // Variables para reportes de iva
    float ventasPorIVA[4] = {0.0, 0.0, 0.0, 0.0};
    float tasasIVA[4];
    tasasIVA[1] = 0.00;
    tasasIVA[2] = 0.12;
    tasasIVA[3] = 0.15;

    // Cargar inventario desde archivo
    int totalProductos = cargarInventario(inventario);

    // Cargar afiliados desde archivo
    int totalAfiliados = cargarAfiliados(Afiliado);
    
    // Bucle principal
    do {
        if(opcion != 0) limpiarPantalla(); 
        
        printf("__________________________________________________________\n");
        printf("                         ABY'S POS                        \n");
        printf("__________________________________________________________\n");
        printf("   Productos cargados: %d\n", totalProductos);
        if (cajaAbierta == 1) {
            printf("   Estado de Caja: ABIERTA | Total: $%.2f\n", totalCaja);
        } else {
            printf("   Estado de Caja: CERRADA\n");
        }
        printf("__________________________________________________________\n");
        printf("1. Modulo de Productos\n");
        printf("2. Modulo de Inventario\n");
        printf("3. Modulo de Ventas\n");
        printf("4. Modulo de Caja\n");
        printf("5. Modulo de Reportes\n");
        printf("6. Listar afiliados\n");
        printf("7. Salir\n");
        printf("__________________________________________________________\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            
            case 1: // Modulo de productos
                limpiarPantalla(); 
                printf("--- MODULO DE PRODUCTOS ---\n");
                printf("1) Registrar nuevo producto\n");
                printf("2) Listar productos\n");
                printf("3) Editar producto\n");
                printf("Seleccione: ");
                scanf("%d", &subOpcion);

                if (subOpcion == 1) {
                    if (totalProductos >= PRO_MAX) {
                        printf("Error: Catalogo lleno.\n");
                    } else {
                        int nuevoID;
                        printf("\n--- REGISTRO ---\n");
                        printf("Ingrese el ID deseado: ");
                        scanf("%d", &nuevoID);

                        if (buscarID(inventario, totalProductos, nuevoID) != -1) {
                            printf("[!] ERROR: El ID %d ya existe.\n", nuevoID);
                        } else {
                            inventario[totalProductos].id = nuevoID;
                            printf("Nombre: ");
                            scanf(" %[^\n]", inventario[totalProductos].nombre);
                            printf("Precio: ");
                            scanf("%f", &inventario[totalProductos].precio);
                            printf("Stock: ");
                            scanf("%d", &inventario[totalProductos].stock);
                            printf("Tipo IVA (1=0%%, 2=12%%, 3=15%%): ");
                            scanf("%d", &inventario[totalProductos].tipoIVA);
                            
                            if(inventario[totalProductos].tipoIVA < 1 || inventario[totalProductos].tipoIVA > 3)
                                inventario[totalProductos].tipoIVA = 2;

                            totalProductos++;
                            //ordenarInventarioPorId(inventario, totalProductos); 
                            guardarInventario(inventario, totalProductos);
                            printf(">> Producto registrado y guardado.\n");
                        }
                    }
                } else if (subOpcion == 2) {
                    printf("\n--- LISTA ---\n");
                    if (totalProductos == 0) printf("Vacio.\n");
                    else {
                        printf("ID   | NOMBRE               | PRECIO | STOCK | IVA\n");
                        for(i = 0; i < totalProductos; i++) {
                            printf("%d  | %-20s | $%.2f  | %-5d | %.0f%%\n",
                                inventario[i].id, inventario[i].nombre, inventario[i].precio,
                                inventario[i].stock, tasasIVA[inventario[i].tipoIVA]*100);
                        }                        
                    }
                } else if (subOpcion == 3) {
                    printf("\nIngrese ID a editar: ");
                    scanf("%d", &idBuscado);
                    
                    encontrado = -1;
                    for(i = 0; i < totalProductos; i++) {
                        if(inventario[i].id == idBuscado) encontrado = i;
                    }

                    if (encontrado != -1) {
                        printf("Editando: %s\n", inventario[encontrado].nombre);
                        printf("Nuevo Precio: "); scanf("%f", &inventario[encontrado].precio);
                        printf("Nuevo Stock: "); scanf("%d", &inventario[encontrado].stock);
                        guardarInventario(inventario, totalProductos);
                        printf("Actualizado.\n");
                    } else printf("No encontrado.\n");
                }
                break;

            case 2: // Modulo de inventario
                limpiarPantalla(); 
                printf("--- INVENTARIO (Stock) ---\n");
                printf("Ingrese ID para ajuste: ");
                scanf("%d", &idBuscado);
                
                encontrado = -1;
                for(i = 0; i < totalProductos; i++) {
                    if(inventario[i].id == idBuscado) encontrado = i;
                }
                // Ajuste de stock
                if (encontrado != -1) {
                    printf("Producto: %s (Actual: %d)\n", inventario[encontrado].nombre, inventario[encontrado].stock);
                    printf("Cantidad a sumar/restar: ");
                    scanf("%d", &cantidad);
                    if (inventario[encontrado].stock + cantidad < 0) printf("Error: Stock negativo.\n");
                    else {
                        inventario[encontrado].stock += cantidad;
                        guardarInventario(inventario, totalProductos);
                        printf("Nuevo stock: %d\n", inventario[encontrado].stock);
                    }
                } else printf("No encontrado.\n");
                break;

            case 3: // Modulo de ventas
                limpiarPantalla(); 
                if (cajaAbierta == 0) printf("Caja Cerrada. Abra caja primero.\n"); 
                else if (totalProductos == 0) printf("Sin productos.\n");
                else { 
                    totalTicket = 0.0; subtotalTicket = 0.0; ivaTicket = 0.0; seguirVendiendo = 's'; itemsTicket = 0;
                    printf("--- NUEVA VENTA ---\n"); 

                    // --- MOSTRAR PRODUCTOS DISPONIBLES ---
                    printf("\n--- DISPONIBLES PARA VENTA ---\n");
                    printf("%-4s | %-20s | %-8s | %s\n", "ID", "NOMBRE", "PRECIO", "STOCK");
                    printf("------------------------------------------------\n");
                    int hayDisponibles = 0;
                    for(int k = 0; k < totalProductos; k++) {
                        if(inventario[k].stock > 0) { // Filtramos solo con stock
                            printf("%-4d | %-20s | $%7.2f | %d\n", 
                                inventario[k].id, inventario[k].nombre, inventario[k].precio, inventario[k].stock);
                            hayDisponibles = 1;
                        }
                    }
                    if(!hayDisponibles) printf(" [!] No hay productos con stock > 0\n");
                    printf("------------------------------------------------\n\n");
                    // -------------------------------------

                    do {
                        printf("Ingrese ID producto: ");
                        scanf("%d", &idBuscado);
                        // Busqueda binaria en venta tambien
                        int idx = buscarID(inventario, totalProductos, idBuscado);

                        if (idx != -1) {
                            printf("Seleccionado: %s ($%.2f)\n", inventario[idx].nombre, inventario[idx].precio);
                            printf("Cantidad: "); scanf("%d", &cantidad);

                            if (cantidad > inventario[idx].stock) printf("Stock insuficiente (Hay %d).\n", inventario[idx].stock);
                            else if (cantidad <= 0) printf("Cantidad invalida.\n");
                            else {
                                subtotalItem = inventario[idx].precio * cantidad;
                                int tipo = inventario[idx].tipoIVA;
                                ivaItem = subtotalItem * tasasIVA[tipo];
                                
                                inventario[idx].stock -= cantidad;
                                guardarInventario(inventario, totalProductos);

                                ventasPorIVA[tipo] += subtotalItem;
                                totalCaja += (subtotalItem + ivaItem);
                                subtotalTicket += subtotalItem;
                                ivaTicket += ivaItem;
                                totalTicket += (subtotalItem + ivaItem);

                                if(itemsTicket < 50) {
                                    ticketProdIndex[itemsTicket] = idx;
                                    ticketCantidad[itemsTicket] = cantidad;  
                                    itemsTicket++;
                                }
                                printf(">> Agregado.\n");
                            }
                        } else printf("Producto no encontrado.\n");

                        printf("¿Otro? (s/n): "); scanf(" %c", &seguirVendiendo);
                    } while (seguirVendiendo == 's' || seguirVendiendo == 'S');

                    if (totalTicket > 0) {
                        // Verificar si el cliente es afiliado
                        ordenarAfiliados(Afiliado, totalAfiliados);
                        printf("Digite el DNI del cliente: ");
                        scanf("%d", &DNICliente);
                        int idxAfiliado = buscarAfiliado(Afiliado, totalAfiliados, DNICliente);
                        if (idxAfiliado != -1) {
                            printf("Cliente afiliado encontrado: %s\n", Afiliado[idxAfiliado].nombre);
                            
                            generarArchivoTicket(ticketProdIndex, ticketCantidad, itemsTicket, inventario, tasasIVA, totalTicket, subtotalTicket, ivaTicket);
                        
                            //Aqui se registra la venta en el cierre de caja
                            registrarVentaEnCierre(ticketProdIndex, ticketCantidad, itemsTicket, inventario);  
                            printf("Venta finalizada y registrada en log. Total: $%.2f\n", totalTicket);
                        } else {
                            printf("Cliente no afiliado.\n");
                        }
                    }
                }
                break;

            case 4: // Modulo de caja
                limpiarPantalla(); 
                printf("--- CAJA ---\n");
                printf("1. Abrir Turno\n");
                printf("2. Cerrar Turno (Generar Reporte)\n");
                printf("Seleccione: ");
                scanf("%d", &subOpcion);
                
                if(subOpcion == 1) { 
                    if(cajaAbierta == 1) printf("La caja ya esta abierta.\n");
                    else {
                        cajaAbierta = 1; 
                        totalCaja = 0; 
                        ventasPorIVA[1]=0; ventasPorIVA[2]=0; ventasPorIVA[3]=0; 
                        printf("Caja ABIERTA correctamente.\n"); 
                    }
                }
                else if(subOpcion == 2) { 
                    if(cajaAbierta == 0) printf("La caja ya esta cerrada.\n");
                    else {
                        // 1. Mostrar en pantalla para el cajero
                        generarCierreCaja(totalCaja, ventasPorIVA, tasasIVA);
                        
                        // 2. Guardar respaldo en archivo para el dueño
                        guardarCierreArchivo(totalCaja, ventasPorIVA, tasasIVA);
                        
                        cajaAbierta = 0; 
                        printf("\nCaja cerrada y reporte guardado. Presione Enter...");
                        getchar(); getchar();
                    }
                }
                break;
            
            case 5: // Reportes rapidos
                limpiarPantalla();
                printf("     Reporte Rapido    \n");
                printf("Total Caja Actual: $%.2f\n", totalCaja);
                break;
                
            case 6:
                for (i = 0; i < totalAfiliados; i++){
                    printf("AFILIADO: ID %d | %s | DNI: %d\n", Afiliado[i].id, Afiliado[i].nombre, Afiliado[i].DNI);
                }
                break;

            case 7: printf("Saliendo...\n"); break;
            default: printf("Opcion no valida.\n");
        }

        if (opcion != 7) {
            printf("\n[1] Volver al Menu: ");
            scanf("%d", &continuar);
        }

    } while (opcion != 7);

    return 0;
}