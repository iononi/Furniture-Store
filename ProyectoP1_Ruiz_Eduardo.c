#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bienes
{
	char nombre[60];
	char modelo[35];
	char descripcion[90];
	float compra;
	float venta;
	char sku[13];
	int stock;
};

struct vendedores
{
	char ID[50];
	float cantidad;//cantidad de dinero vendido
};

struct ventas
{
	char articulo[60];
	char vendedor[50];
	float pcompra;
	float pventa;
	int cantidad;//cantidad de articulos vendidos
};

void leerArchivo(FILE **fp, struct bienes *p);
void limpiar(char *str);
int buscarProducto(FILE **fp, struct bienes *q, char *str);

int main(int argc, char const *argv[])
{
	struct bienes muebles;
	struct vendedores vendedor;
	struct ventas venta;
	FILE *fi, *fv, *frv, *ftemp;
	int op, cant, existe = 0, vendidos;
	char aux[50];

	do
	{
		printf("TU EMPRESA %c\n", 169);
		printf("\n-==== MENU DE OPCIONES ====-\n");
		printf("[1] Registrar un producto\n");
		printf("[2] Eliminar un producto\n");
		printf("[3] Mostrar inventario\n");
		printf("[4] Agregar stock a un producto\n");
		printf("[5] Dar de alta a un vendedor\n");
		printf("[6] Dar de baja a un vendedor\n");
		printf("[7] Realizar una venta\n");
		printf("[8] Recuento de ventas\n");
		printf("[9] Salir\n\n");

			fflush(stdin);
			printf("Opcion >> ");
			scanf("%d", &op);	
		
		switch(op)
		{
			case 1: printf("Cantidad de productos a registrar: ");
					scanf("%d", &cant); 
					if (!(fi = fopen("Inventario.bin", "ab")))
					{
						printf("Ha ocurrido un error al abrir el archivo.\n\n");
						continue;
					}
					for (int i = 0; i < cant; ++i)
					{
						printf("Ingrese el nombre del producto: "); fflush(stdin);
						fgets(muebles.nombre, 60, stdin); strtok(muebles.nombre, "\n");
						printf("Ingrese el modelo del producto: "); fflush(stdin);
						fgets(muebles.modelo, 35, stdin); strtok(muebles.modelo, "\n");
						printf("Ingrese una breve descripcion: "); fflush(stdin);
						fgets(muebles.descripcion, 90, stdin); strtok(muebles.descripcion, "\n");
						printf("Ingrese el precio de compra: ");
						scanf("%f", &(muebles.compra));
						printf("Ingrese el precio de venta: ");
						scanf("%f", &(muebles.venta));
						printf("Ingrese el codigo SKU: "); fflush(stdin);
						fgets(muebles.sku, 13, stdin); strtok(muebles.sku, "\n");
						do
						{
							printf("Ingrese la cantidad de stock del producto: ");
							scanf("%d", &muebles.stock);
						}while(muebles.stock <= 0);
						fwrite(&muebles, sizeof(struct bienes), 1, fi);
						printf("\n");
					}
					fclose(fi);
					printf("\nSe han registrado los datos correctamente.\n\n");
			break;
			case 2:	if(!(fi = fopen("Inventario.bin", "rb")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'Inventario.bin'\n\n");
						continue;
					}
					if (!(ftemp = fopen("temporal.bin", "wb")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'temporal.bin'\n\n");
						fclose(fi);
						continue;
					}
					printf("Ingrese el SKU del producto que desea eliminar: ");
					fflush(stdin);
					fgets(aux, 50, stdin);
					strtok(aux, "\n");
					fread(&muebles, sizeof(struct bienes), 1, fi);
					while(!feof(fi))
					{
						if(strcmp(muebles.sku, aux) != 0)
						{
							fwrite(&muebles, sizeof(struct bienes), 1, ftemp);
							fread(&muebles, sizeof(struct bienes), 1, fi);
						}
						else
							fread(&muebles, sizeof(struct bienes), 1, fi);
					}
					fclose(ftemp);
					fclose(fi);
					limpiar(aux);
					if (remove("Inventario.bin") != 0)
					{
						printf("No se ha podido eliminar el archivo 'Inventario.bin'\n\n");
						continue;
					}
					if (rename("temporal.bin", "Inventario.bin") != 0)
					{
						printf("No se ha podido renombrar el archivo 'temporal.bin'\n\n");
						continue;
					}
					printf("\nSe ha eliminado el producto satisfactoriamente.\n\n");
			break;
			case 3: leerArchivo(&fi, &muebles);
			break;
			case 4:	if(buscarProducto(&fi, &muebles, aux) != 0)
						continue;
					do
					{
						printf("Ingrese la cantidad que desea agregar al inventario: ");
						scanf("%d", &cant);
					}while(cant <= 0);
					muebles.stock += cant;
					fseek(fi, ftell(fi) - sizeof(struct bienes), SEEK_SET);
					fwrite(&muebles, sizeof(struct bienes), 1, fi);
					printf("Se han agregado %d nuevos articulos al producto.\n\n", cant);
					fclose(fi);
			break;
			case 5:	if (!(fv = fopen("Vendedores.bin", "ab")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'Vendedores.bin'.\n\n");
						continue;
					}
					printf("\nIdentificador del vendedor: ");
					fflush(stdin);
					fgets(vendedor.ID, 50, stdin);
					strtok(vendedor.ID, "\n");
					vendedor.cantidad = 0;
					fwrite(&vendedor, sizeof(struct vendedores), 1, fv);
					printf("\nLos datos del vendedor han sido registrados correctamente.\n\n");
					fclose(fv);
			break;
			case 6: if (!(fv = fopen("Vendedores.bin", "rb")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'Vendedores.bin'.\n\n");
						continue;
					}
					if (!(ftemp = fopen("temporal.bin","wb")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'temporal.bin'\n\n");
						fclose(fv);
						continue;
					}

					printf("Ingrese el identificador del vendedor que desea dar de baja: ");
					fflush(stdin);
					fgets(aux, 50, stdin);
					strtok(aux, "\n");
					fread(&vendedor, sizeof(struct vendedores), 1, fv);
					while(!feof(fv))
					{
						if (strcmp(vendedor.ID, aux) != 0)
						{
							fwrite(&vendedor, sizeof(struct vendedores), 1, ftemp);
							fread(&vendedor, sizeof(struct vendedores), 1 , fv);
						}
						else
							fread(&vendedor, sizeof(struct vendedores), 1, fv);
					}
					limpiar(aux);
					fclose(ftemp);
					fclose(fv);
					if(remove("Vendedores.bin") != 0)
					{
						printf("No se ha podido eliminar el archivo 'Vendedores.bin'.\n\n");
						continue;
					}
					if(rename("temporal.bin", "Vendedores.bin") != 0)
					{
						printf("No se ha podido renombrar el archivo temporal.\n\n");
						continue;
					}

					printf("Se ha dado de baja al vendedor satisfactoriamente.\n\n");
			break;
			case 7:	if(!(fv = fopen("Vendedores.bin", "r+b")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'Vendedores.bin'\n\n");
						continue;
					}

					do
					{
						printf("Ingrese el identificador del vendedor en turno: ");
						fflush(stdin);
						fgets(aux, 50, stdin);
						strtok(aux, "\n");
						fread(&vendedor, sizeof(struct vendedores), 1, fv);
						while(!feof(fv))
						{
							if(strcmp(vendedor.ID, aux) == 0)
							{
								existe = 1;
								printf("\nBienvenido, %s\n", vendedor.ID);
								break;
							}
							fread(&vendedor, sizeof(struct vendedores), 1, fv);
						}
						if (existe == 0)
						{
							printf("No se encontro un vendedor con dicho nombre.\n\n");
							limpiar(aux);
							rewind(fv);
						}
					}while(existe == 0);
					limpiar(aux);
					if (buscarProducto(&fi, &muebles, aux) != 0)
					{
						fclose(fv);
						continue;
					}
					do
					{
						printf("\nIngrese la cantidad de articulos que desea comprar: ");
						scanf("%d", &vendidos);
					}while(vendidos <= 0 || vendidos > muebles.stock);
					printf("%cDesea continuar con la compra de '%s'? [S/N] \n", 168, muebles.nombre);
					fflush(stdin);
					fgets(aux, 2, stdin);
					if (strncmp(aux, "S", 1) == 0 || strncmp(aux, "s", 1) == 0)
					{
						if (!(frv = fopen("Registro de ventas.bin", "ab")))
						{
							printf("Ha ocurrido un error al abrir el archivo 'Registro de ventas.bin'\n\n");
							continue;
						}
						strcpy(venta.articulo, muebles.nombre);
						strcpy(venta.vendedor, vendedor.ID);
						venta.pcompra = muebles.compra;
						venta.pventa = muebles.venta;
						venta.cantidad = vendidos;
						fwrite(&venta, sizeof(struct ventas), 1, frv);
						printf("Articulo vendido: %s \n", muebles.nombre);
						printf("Precio: %.2f \n", muebles.venta);
						printf("Articulos vendidos: %d \n", vendidos);
						printf("TOTAL: $%.2f \n\n", vendidos * muebles.venta);
						printf("La compra ha finalizado exitosamente.\n\n");
						fclose(frv);
						vendedor.cantidad += (vendidos * muebles.venta);
						fseek(fv, ftell(fv) - sizeof(struct vendedores), SEEK_SET);
						fwrite(&vendedor, sizeof(struct vendedores), 1, fv);
						muebles.stock -= vendidos;
						fseek(fi, ftell(fi) - sizeof(struct bienes), SEEK_SET);
						fwrite(&muebles, sizeof(struct bienes), 1, fi);
						fclose(fi);
						fclose(fv);
						limpiar(aux);
					}
					else
					{
						printf("La operacion se ha cancelado.\n\n");
						fclose(fi);
						fclose(fv);
						existe = 0;
						limpiar(aux);
						continue;
					}
			break;
			case 8:	if (!(frv = fopen("Registro de ventas.bin", "rb")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'Registro de ventas.bin'\n\n");
						continue;
					}
					int total_ventas = 0, i = 1, cont = 0;
					float ganancias = 0;
					fread(&venta, sizeof(struct ventas), 1, frv);
					while(!feof(frv))
					{
						total_ventas += venta.cantidad;
						printf("\nProducto vendido: %s \n", venta.articulo);
						printf("Vendedor: %s \n", venta.vendedor);
						printf("Precio compra: %.2f \n", venta.pcompra);
						printf("Precio: %.2f \n", venta.pventa);
						printf("Articulos vendidos: %d \n", venta.cantidad);
						printf("Margen de ganancia Producto %d: %.2f \n", i++, venta.cantidad * (venta.pventa - venta.pcompra));
						ganancias += venta.cantidad * (venta.pventa - venta.pcompra);
						fread(&venta, sizeof(struct ventas), 1, frv);
					}
					fclose(frv);
					printf("\nTotal de articulos vendidos: %d \n", total_ventas);
					printf("\nGanancias Totales: %.2f \n\n", ganancias);
					if (!(fv = fopen("Vendedores.bin", "r+b")))
					{
						printf("Ha ocurrido un error al abrir el archivo 'Vendedores.bin'\n\n");
						continue;
					}
					fread(&vendedor, sizeof(struct vendedores), 1, fv);
					while(!feof(fv))
					{
						cont++;
						fread(&vendedor, sizeof(struct vendedores), 1, fv);
					}
					rewind(fv);//fseek(fv, 0, SEEK_SET)
					i = 0;
					struct vendedores *ranking = (struct vendedores *)malloc(cont*sizeof(float));
					if(ranking == NULL)
					{
						printf("No se ha podido reservar la memoria requerida.\n\n");
						continue;
					}
					while(!feof(fv))
					{
						fread(&vendedor, sizeof(struct vendedores), 1, fv);
						ranking[i++] = vendedor;
					}
					for (int i = 0; i < cont - 1; ++i)//bubble sort
					{
						for (int j = 0; j < cont - 1; ++j)
						{
							if (ranking[j].cantidad < ranking[j + 1].cantidad)
							{
								float aux = ranking[j].cantidad;
								ranking[j].cantidad = ranking[j + 1].cantidad;
								ranking[j + 1].cantidad = aux;
							}
						}
					}
					printf("\nEste es el ranking de vendedores con mas ventas\n");
					for (int i = 0; i < cont; ++i)
					{
						printf("Vendedor: %s \n", ranking[i].ID);
						printf("Monto vendido: %.2f\n\n", ranking[i].cantidad);
					}
					fclose(fv);
			break;
			case 9: printf("\n %c TU EMPRESA. Todos los derechos reservados.\n\n", 184);
			break;
			default: printf("Ingrese una opcion valida del menu.\n\n");
		}
	}while(op!=9);
	return 0;
}

void leerArchivo(FILE **fp, struct bienes *p)
{
	if (!(*fp = fopen("Inventario.bin", "rb")))
	{
		printf("Ha ocurrido un error al abrir el archivo.\n\n");
		return;
	}
	fread(p, sizeof(struct bienes), 1, *fp);
	while(!feof(*fp))
	{
		printf("\nNombre: %s \n", p->nombre);
		printf("Modelo: %s \n", p->modelo);
		printf("Descripcion: %s \n", p-> descripcion);
		printf("Precio de compra: %.2f\n", p->compra);
		printf("Precio de venta: %.2f\n", p->venta);
		printf("SKU: %s \n", p->sku);
		printf("Existencias disponibles: %d\n", p->stock);
		fread(p, sizeof(struct bienes), 1, *fp);
	}
	fclose(*fp);
}

int buscarProducto(FILE **fp, struct bienes *q, char *str)
{
	int existe = 0;
	if (!(*fp = fopen("Inventario.bin", "r+b")))
		{
			printf("Ha ocurrido un error al abrir el archivo 'Inventario.bin'.\n\n");
			return -1;
		}
	do
	{
		printf("\nIngrese el SKU del producto: ");
		fflush(stdin);
		fgets(str, 13, stdin);
		strtok(str, "\n");
		fread(q, sizeof(struct bienes), 1, *fp);
		while(!feof(*fp))
		{
			if (strcmp(q->sku, str) == 0)
			{
				existe = 1;
				printf("\nProducto: %s \n", q->nombre);
				printf("Modelo: %s \n", q->modelo);
				printf("Descripcion: %s \n", q->descripcion);
				printf("Precio: %.2f \n", q->venta);
				printf("SKU: %s \n", q->sku);
				printf("Articulos disponibles: %d \n\n", q->stock);
				break;
			}
			fread(q, sizeof(struct bienes), 1, *fp);
		}
		if (existe == 0)
			{
				printf("No se encontro un producto con el SKU %s\n", str);
				limpiar(str);
				fseek(*fp, 0, SEEK_SET);
			}
	}while(existe == 0);
	limpiar(str);
	return 0;
}

void limpiar(char *str)
{
	for (int i = 0; i < 50; ++i)
		str[i] = '\0';
}