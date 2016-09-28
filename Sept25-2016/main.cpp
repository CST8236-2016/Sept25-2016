#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

int main(int argc, char *argv)
{
  sf::RenderWindow window(sf::VideoMode(900, 400), "Template", sf::Style::Titlebar | sf::Style::Close);

  sf::Event evt;
  sf::Clock appTimer;

  /* Loading textures */
  sf::Texture dogeTexture;
  dogeTexture.loadFromFile("res/doge.jpg");

  sf::Texture astrocatTexture;
  astrocatTexture.loadFromFile("res/astrocat.jpg");

  sf::Texture cloudsTexture;
  cloudsTexture.loadFromFile("res/tiling_clouds.jpg");
  cloudsTexture.setRepeated(true); /* Tells the texture that it can repeated if something tries to
                                    * draw it outside its bounds. */

  /* Loading sprites */
  sf::Sprite dogeSprite;
  dogeSprite.setTexture(dogeTexture);
  dogeSprite.setScale(0.5f, 0.5f);

  sf::Sprite astrocatSprite;
  astrocatSprite.setTexture(astrocatTexture);

  /* Non-tiling sprites */
  sf::Sprite nonTilingCloudsSprite;
  nonTilingCloudsSprite.setTexture(cloudsTexture);

  sf::Sprite nonTilingSecondCloudsSprite;
  nonTilingSecondCloudsSprite.setTexture(cloudsTexture);

  /* Tiling sprite */
  sf::Sprite tilingCloudsSprite;
  tilingCloudsSprite.setTexture(cloudsTexture);
  // We tell the sprite that it's going to draw the cloud texture at the size of our window.
  tilingCloudsSprite.setTextureRect(sf::IntRect(0, 0, window.getSize().x, window.getSize().y));

  /* Loading Audio */
  //sf::SoundBuffer buffer;
  //buffer.loadFromFile("res/04 Sharks In Danger.flac");
  //sf::Sound sound(buffer);

  //sf::Music music;
  //music.openFromFile("res/04 Sharks In Danger.flac");

  /* The size of our sprite's drawable area, as represented by a rectangle. This will be updated
   * once per second, as we're dealing with floats for time, and ints for location/size, so we risk
   * truncating information otherwise. */
  sf::IntRect backgroundRect = tilingCloudsSprite.getTextureRect();
  float secondTimer = 0.0f; // elapsed time per second.
  float movementSpeed = 150.0f; // scrolling movement speed.

  /* Size of our sprite's drawable area. */
  //float width = 0.0f;
  //float height = 0.0f;

  while (window.isOpen()) {
    float deltaTime = appTimer.restart().asSeconds();

    while (window.pollEvent(evt)) {
      if (evt.type == sf::Event::Closed) {
        window.close();
      }
      else if (evt.type == sf::Event::KeyPressed) {
        //sound.play();
        //music.play();
      }
    }

    /* Used to scale the size of our sprite's drawable area. */
    /*width += (45.0f * deltaTime);
    height += (45.0f * deltaTime);
    nonTilingCloudsSprite.setTextureRect(sf::IntRect(0, 0, width, height));*/

    //
    secondTimer += deltaTime;

    /* So this part is a bit strange. We can't easily multiply the elapsed time (float) by an
     * amount that we want the texture rectangle (int) to move, as the elapsed time could be so small
     * that the value gets truncated to 0 (eg. 0.002f seconds * movementSpeed = 0 * movementSpeed).
     * What I've chosen to do is to store the location of the rectangle once per second, and then create an
     * intermediate rectangle from the stored one and offset it by the amount we want to move per second
     * multiplied by time that's elapsed. We can accrue the elapsed time (secondTimer) and multiply it by
     * our movementSpeed, and add it to the intermediate rectangle. Then, once we've reached one second,
     * we take that second off of our timer, add the full movementSpeed to our stored rectangle, and repeat. */
    sf::IntRect deltaRect = backgroundRect;
    deltaRect.left = backgroundRect.left + (int)(movementSpeed * secondTimer);
    tilingCloudsSprite.setTextureRect(deltaRect);

    // Check if more than a second has elapsed, as that's all we're checking.
    if (secondTimer > 1.0f)
    {
      /* Remove a second from timer (we can't set it to 0 because there's a chance that a small
       * fraction of a second has elapsed, and we don't want to lose that). eg. 1.001f */
      secondTimer -= 1.0f;
      backgroundRect.left += (int)movementSpeed; // Add on the appropriate offset.

      /* We don't want our offset to get too big, so what we do is check to see if it's larger than our
       * texture. If it is, we can safely subtract the size of our texture from the rectangle's offset and
       * see the exact same point on the texture. (if width = 100, then we don't want the offset to be 1300
       * when 1300 looks exactly like 100 because of the repeating texture; this makes our numbers much easier
       * to work with. */
      if (backgroundRect.left >= (float)cloudsTexture.getSize().x) {
        backgroundRect.left -= cloudsTexture.getSize().x;
      }
    }

    // Move our glorious sprite.
    //dogeSprite.move(10.0f * deltaTime, 0);

    //nonTilingCloudsSprite.move(-movementSpeed * deltaTime, 0.0f);

    float textureWidth = (float)cloudsTexture.getSize().x;
    // Check if the position of our sprite + width is < 0 because this means that our image is entirely off screen.
    if (nonTilingCloudsSprite.getPosition().x + textureWidth <= 0.0f)
    {
      // Move it forward by an image-width.
      nonTilingCloudsSprite.setPosition(nonTilingCloudsSprite.getPosition().x + textureWidth, 0.0f);
      //cloudsSprite.setPosition(0.0f, 0.0f);
    }

    // The current position of our sprite + its width, consolidated for readabilty.
    float combinedX = nonTilingCloudsSprite.getPosition().x + textureWidth;
    nonTilingSecondCloudsSprite.setPosition(combinedX, nonTilingCloudsSprite.getPosition().y);

    // Tell the window to 'clear' its contents (paint over it with the background colour).
    window.clear();

    //Draw stuff
    //window.draw(nonTilingCloudsSprite);
    //window.draw(nonTilingSecondCloudsSprite);
    window.draw(tilingCloudsSprite);

    // window.draw(dogeSprite);

    //window.draw(astrocatSprite);

    // Tell the window to take everything it's drawn into its offscreen buffer and display it onscreen.
    window.display();
  }

  return 0;
}