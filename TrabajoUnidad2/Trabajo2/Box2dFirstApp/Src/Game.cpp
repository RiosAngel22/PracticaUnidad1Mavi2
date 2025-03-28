#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    Color limiteColor = Color::Transparent;

    // Dibujar el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(limiteColor);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    //dibujar las paredes
    sf::RectangleShape wallShape(sf::Vector2f(5, 500));
    wallShape.setFillColor(limiteColor);
    wallShape.setPosition(0, 0);
    wnd->draw(wallShape);

    sf::RectangleShape wallShape2(sf::Vector2f(5, 500));
    wallShape2.setFillColor(limiteColor);
    wallShape2.setPosition(95, 0);
    wnd->draw(wallShape2);

    sf::RectangleShape ceilingShape(sf::Vector2f(500, 5));
    ceilingShape.setFillColor(limiteColor);
    ceilingShape.setPosition(0, 0);
    wnd->draw(ceilingShape);

    // Dibujar el cuerpo de control (círculo)
    sf::CircleShape controlShape(5);
    controlShape.setFillColor(sf::Color::Magenta);
    controlShape.setPosition(controlBody->GetPosition().x-5, controlBody->GetPosition().y-5);
    wnd->draw(controlShape);
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
            break;
        }
    }
    controlBody->SetAwake(true);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        controlBody->SetLinearVelocity(b2Vec2(-30.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Right))
        controlBody->SetLinearVelocity(b2Vec2(30.0f, 0.0f));
    if (Keyboard::isKeyPressed(Keyboard::Down))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, 30.0f));
    if (Keyboard::isKeyPressed(Keyboard::Up))
        controlBody->SetLinearVelocity(b2Vec2(0.0f, -30.0f));
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes estáticas del mundo físico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

    b2Body* wallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    wallBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    b2Body* wallBody2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 200);
    wallBody2->SetTransform(b2Vec2(100.0f, 0.0f), 0.0f);

    b2Body* ceilingBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 200, 10);
    ceilingBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);

    // Crear un círculo que se controlará con el teclado
    controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 5.0f);
    controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{ }