// Create variables 
var   wh = $(window).height(),
      $header = $('.header'),
      $hero = $('.hero'),
      $heroImg = $('.hero-image'),
      $heroTitle = $('.hero-title'),
      $heroTitleH1 = $('.hero-title h1'),
      $heroTitleH5 = $('.hero-title h5'),
      $container = $('.container'),
      $innerHeader = $('.inner-header'),
      $containerImg = $('.container img');

// Init
var ctrl = new ScrollMagic.Controller({
  globalSceneOptions: {
    triggerHook: 'onLeave'
  }
});

// Create scene
$hero.each(function() {
  new ScrollMagic.Scene({
      duration: '80%',

    
    })
    .setPin(this)
    .addTo(ctrl);
});


// Create  timeline and animations
var fadeScroll = new TimelineMax();
fadeScroll
  .set($heroImg, {scale: 1.1})
  .set($heroTitle, {perspective:800, transformStyle:'preserve-3d'})
  .set($container, {yPercent: 41, perspective:800, transformStyle:'preserve-3d'}) 
  .to($heroImg, 4, {yPercent: -5,scale: 0.8, ease:SlowMo.easeOut}, '0')
  .to($heroTitleH1, 4, {scale:0.9, rotationX:-10, rotationY:10}, '0')
  .to($heroTitleH5, 4, {scale:0.9,x:-30, rotationX:-10, rotationY:10}, '0')
  .fromTo($heroImg, 2, {opacity: 1}, {opacity: 0}, '=-.9')
  .to($container, .5, {autoAlpha: 1, y:-50,ease:SlowMo.easeOut}, '=-0.7')
  .to($header, 1, {color: 'black'}, '+1.4')
  

// Create scene
new ScrollMagic.Scene({
    duration: '120%'
  })
  .setTween(fadeScroll)
  .triggerElement($hero)
  .addTo(ctrl);


  
//Image transition loop 
$($containerImg).each(function(){ 
  
   // Create variables
   var controller = new ScrollMagic.Controller();
   var img = this;
  
   // Create  timeline and animations
   var imageIn = new TimelineMax(); 
       imageIn
      .set(img, {scale:.8})
      .to(img, 2, {autoAlpha:1,scale:1,rotationX:0, ease: Power0.easeNone });
  
   // Create scene
   var scene = new ScrollMagic.Scene({
          duration: '100%',
            triggerElement:img,
            offset: -wh*0.9
        })     
        .setTween(imageIn)
        .addTo(controller); 
  
});