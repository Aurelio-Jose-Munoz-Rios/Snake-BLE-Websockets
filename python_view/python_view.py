import asyncio
from websockets.server import serve

async def handle_client(websocket):  # Solo 1 parámetro
    print("Cliente conectado")
    try:
        async for message in websocket:
            print(f"Comando recibido: {message}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        print("Cliente desconectado")

async def main():
    async with serve(handle_client, "0.0.0.0", 8765):
        print("Servidor WebSocket corriendo en puerto 8765")
        await asyncio.Future()

if __name__ == "__main__":
    asyncio.run(main())
