import asyncio
import websockets
import pygame
import json

# Configuración de Pygame
pygame.init()
WIDTH, HEIGHT = 600, 450
CELL_SIZE = 30
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Snake Game - ESP32")
clock = pygame.time.Clock()

# Colores
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)

game_state = {"type": "waiting"}

def draw_game():
    screen.fill(BLACK)
    
    if game_state.get("type") == "waiting":
        font = pygame.font.Font(None, 48)
        text = font.render("Presiona un boton para iniciar", True, WHITE)
        screen.blit(text, (50, HEIGHT//2))
    
    elif game_state.get("type") == "gameover":
        font = pygame.font.Font(None, 64)
        text = font.render("GAME OVER", True, RED)
        screen.blit(text, (150, HEIGHT//2 - 50))
        score_text = font.render(f"Score: {game_state.get('score', 0)}", True, WHITE)
        screen.blit(score_text, (200, HEIGHT//2 + 20))
    
    elif game_state.get("type") == "game":
        # Dibujar serpiente
        snake = game_state.get("snake", [])
        for i, (x, y) in enumerate(snake):
            color = GREEN if i == 0 else BLUE
            pygame.draw.rect(screen, color, (x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE-2, CELL_SIZE-2))
        
        # Dibujar comida
        food = game_state.get("food", [0, 0])
        pygame.draw.rect(screen, RED, (food[0]*CELL_SIZE, food[1]*CELL_SIZE, CELL_SIZE-2, CELL_SIZE-2))
        
        # Mostrar puntaje
        font = pygame.font.Font(None, 36)
        score_text = font.render(f"Score: {game_state.get('score', 0)}", True, WHITE)
        screen.blit(score_text, (10, 10))
    
    pygame.display.flip()

async def handle_websocket(websocket):
    global game_state
    print("ESP32 conectado")
    
    try:
        async for message in websocket:
            game_state = json.loads(message)
            print(f"Estado: {game_state.get('type')}")
            
    except Exception as e:
        print(f"Error: {e}")

async def game_loop():
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                return
        
        draw_game()
        clock.tick(30)
        await asyncio.sleep(0.01)

async def main():
    async with websockets.serve(handle_websocket, "0.0.0.0", 8765):
        print("Servidor Snake corriendo en puerto 8765")
        await game_loop()

if __name__ == "__main__":
    asyncio.run(main())
