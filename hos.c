#include <zx11/zxutil.h>
#include <zx11/zxinput.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <pthread.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

cairo_surface_t *cairo_surface;
cairo_t *cairo;

zxWindow win;

bool hos_pause = false;

void hos_cairo_create(void)
{
  cairo_surface = cairo_xlib_surface_create( zxdisplay, zxWindowBody(&win), zxvisual, zxWindowWidth(&win), zxWindowHeight(&win) );
  cairo = cairo_create( cairo_surface );
}

void hos_cairo_destroy(void)
{
  cairo_destroy( cairo );
  cairo_surface_destroy( cairo_surface );
}

void hos_draw_background(double width, double height)
{
  cairo_set_source_rgb( cairo, 0, 0, 0 );
  cairo_rectangle( cairo, 0, 0, width, height );
  cairo_fill( cairo );
}

void hos_draw_cross(double width, double height, int w, int h)
{
  cairo_set_source_rgb( cairo, 1, 1, 1 );
  cairo_set_line_width( cairo, 1 );
  cairo_move_to( cairo, -width/4, 0 );
  cairo_rel_line_to( cairo, width/2, 0 );
  cairo_move_to( cairo, 0, -width/4 );
  cairo_rel_line_to( cairo, 0, width/2 );
  cairo_stroke( cairo );
  cairo_set_line_width( cairo, 3 );
  cairo_move_to( cairo, width/2, 0 );
  cairo_rel_line_to( cairo, -w, 0 );
  cairo_move_to( cairo,-width/2, 0 );
  cairo_rel_line_to( cairo, w, 0 );
  cairo_move_to( cairo, 0, height/2 );
  cairo_rel_line_to( cairo, 0, -h );
  cairo_move_to( cairo, 0,-height/2 );
  cairo_rel_line_to( cairo, 0, h );
  cairo_stroke( cairo );
}

void hos_draw_frame(double width, double height, double red_rate, double white_rate)
{
  /* red rectangle */
  cairo_set_source_rgba( cairo, 1, 0, 0, red_rate );
  cairo_rectangle( cairo, -width/2, -height/2, width, height );
  cairo_fill( cairo );
  /* white rectangle */
  cairo_set_source_rgba( cairo, 1, 1, 1, white_rate );
  cairo_rectangle( cairo, -width/2, -height/2, width, height );
  cairo_fill( cairo );
  /* cross */
  cairo_set_source_rgb( cairo, 1, 1, 1 );
  cairo_set_line_width( cairo, 1 );
  cairo_rectangle( cairo, -width/2, -height/2, width, height );
  cairo_stroke( cairo );
  cairo_set_line_width( cairo, 3 );
  cairo_move_to( cairo, -width/2,-height/2+height/16 );
  cairo_rel_line_to( cairo, 0, -height/16 );
  cairo_rel_line_to( cairo, width/16, 0 );
  cairo_move_to( cairo, -width/2, height/2-height/16 );
  cairo_rel_line_to( cairo, 0, height/16 );
  cairo_rel_line_to( cairo, width/16, 0 );
  cairo_move_to( cairo, width/2,-height/2+height/16 );
  cairo_rel_line_to( cairo, 0, -height/16 );
  cairo_rel_line_to( cairo, -width/16, 0 );
  cairo_move_to( cairo, width/2, height/2-height/16 );
  cairo_rel_line_to( cairo, 0, height/16 );
  cairo_rel_line_to( cairo, -width/16, 0 );
  cairo_stroke( cairo );
}

void hos_draw_h(double width, double height, double red_rate)
{
  cairo_set_source_rgb( cairo, red_rate, 0, 0 );
  cairo_set_line_width( cairo, 4 );
  cairo_move_to( cairo, -width/2+4, -height/2 );
  cairo_rel_line_to( cairo, 0, height );
  cairo_move_to( cairo, width/2-4, -height/2 );
  cairo_rel_line_to( cairo, 0, height );
  cairo_move_to( cairo,-width/2+4, 0 );
  cairo_line_to( cairo, width/2-4, 0 );
  cairo_stroke( cairo );
}

void hos_draw_o(double width, double height, double red_rate)
{
  cairo_set_source_rgb( cairo, red_rate, 0, 0 );
  cairo_set_line_width( cairo, 4 );
  cairo_arc( cairo, 0, 0, width/2-5, 0, PI*2 );
  cairo_stroke( cairo );
}

void hos_draw_s(double width, double height, double red_rate)
{
  cairo_set_source_rgb( cairo, red_rate, 0, 0 );
  cairo_set_line_width( cairo, 4 );
  cairo_arc( cairo, 0, -height/4, height/4, PI/2, 3*PI/2 );
  cairo_stroke( cairo );
  cairo_arc( cairo, 0,  height/4, height/4,-PI/2, PI/2 );
  cairo_stroke( cairo );
}

const char label1[] = "Hyper Operating System";
const char label2[] = "Ver 1.0";
const char label3[] = "for ALL LABORS";
const char label4[] = "(C) 1999";
const char label5[] = "Shinohara Heavy Industry";

#define FONT_NAME "Arial"

void hos_draw_label_one(const char *label, int n, int y)
{
  char buf[BUFSIZ];
  cairo_text_extents_t te;

  strncpy( buf, label, n );
  buf[n] = '\0';
  cairo_text_extents( cairo, label, &te );
  cairo_move_to( cairo, te.x_bearing - te.width/2, y + te.y_bearing - te.height/2 );
  cairo_show_text( cairo, buf );
}

void hos_draw_label(double width, double height, int count)
{
  static int n = 0;
  int n1, n2, n3, n4, n5, nl;

  n1 = strlen(label1);
  n2 = strlen(label2);
  n3 = strlen(label3);
  n4 = strlen(label4);
  n5 = strlen(label5);
  cairo_set_source_rgba( cairo, 1, 1, 0, 1 );
  cairo_select_font_face( cairo, FONT_NAME, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
  cairo_set_font_size( cairo, height/20 );
  if( count == 0 ) n = 0;
  if( ( nl = ++n ) <= n1 ){
    hos_draw_label_one( label1, nl, -7*height/20 );
  } else{
    hos_draw_label_one( label1, n1, -7*height/20 );
    if( ( nl -= n1 ) <= n2 ){
      hos_draw_label_one( label2, nl, -6*height/20 );
    } else{
      hos_draw_label_one( label2, n2, -6*height/20 );
      if( ( nl -= n2 ) <= n3 ){
        hos_draw_label_one( label3, nl, -5*height/20 );
      } else{
        hos_draw_label_one( label3, n3, -5*height/20 );
        if( ( nl -= n3 ) <= n4 ){
          hos_draw_label_one( label4, nl,  9*height/20 );
        } else{
          hos_draw_label_one( label4, n4,  9*height/20 );
          if( ( nl -= n4 ) <= n5 ){
            hos_draw_label_one( label5, nl, 10*height/20 );
          } else
            hos_draw_label_one( label5, n5, 10*height/20 );
        }
      }
    }
  }
}

#define HOS_T 10
#define HOS_STOP0 1.5
#define HOS_STOP1 2
#define HOS_STOP2 2.5
#define HOS_STOP3 3
#define HOS_STOP4 4
#define HOS_LABEL_T 200

#define HOS_BLINK_T 5
#define HOS_AUTH_T 500
#define HOS_INTERVAL_NSEC 50

void hos_draw(double width, double height, int count)
{
  double r, a;
  cairo_matrix_t matrix0, matrix1, matrix2, matrix3;

  cairo_identity_matrix( cairo );
  hos_draw_background( width, height );
  cairo_translate( cairo, width/2, height/2 );
  cairo_get_matrix( cairo, &matrix0 );
  hos_draw_cross( width, height, width/4*0.9, ( height - width/2 )/2*0.9 );
  if( count == 0 ) return;
  if( count > 0 && count <= HOS_T*HOS_STOP3 ){
    r = (double)count / HOS_T;
    a = zMin( r/HOS_STOP0, 1 );
    cairo_scale( cairo, a, a );
    cairo_rotate( cairo, -PI*(1-a) );
    hos_draw_frame( width/2, width/2, 0.8, 0 );
    cairo_set_matrix( cairo, &matrix0 );
    a = zMin( r/HOS_STOP1, 1 );
    cairo_scale( cairo, a, a );
    cairo_rotate( cairo, -PI*(1-a) );
    cairo_get_matrix( cairo, &matrix1 );
    hos_draw_frame( 2*width/11, 2*width/11, 0, 0.4 );
    cairo_set_matrix( cairo, &matrix0 );
    a = zMin( r/HOS_STOP2, 1 );
    cairo_scale( cairo, a, a );
    cairo_rotate( cairo, -PI*(1-a) );
    cairo_get_matrix( cairo, &matrix2 );
    hos_draw_frame( width/6, width/6, 0, 0.4 );
    cairo_set_matrix( cairo, &matrix0 );
    a = zMin( r/HOS_STOP3, 1 );
    cairo_scale( cairo, a, a );
    cairo_rotate( cairo, -PI*(1-a) );
    cairo_get_matrix( cairo, &matrix3 );
    hos_draw_frame( width/4, width/4, 0, 0.4 );
    cairo_set_matrix( cairo, &matrix1 );
    hos_draw_h( 2*width/11, 2*width/11, 0 );
    cairo_set_matrix( cairo, &matrix2 );
    hos_draw_o( width/6, width/6, 0 );
    cairo_set_matrix( cairo, &matrix3 );
    hos_draw_s( width/4, width/4, 0 );
  } else
  if( count <= HOS_T*HOS_STOP4 + HOS_LABEL_T ){
    r = zMin( (double)( count - HOS_T*HOS_STOP3 ) / HOS_T, 1 );
    hos_draw_frame( width/2, width/2, 0.8*(1-r), 0 );
    hos_draw_frame( 2*width/11, 2*width/11, 0, 0.4*(1-r) );
    hos_draw_frame( width/6, width/6, 0, 0.4*(1-r) );
    hos_draw_frame( width/4, width/4, 0, 0.4*(1-r) );
    hos_draw_h( 2*width/11, 2*width/11, r );
    hos_draw_o( width/6, width/6, r );
    hos_draw_s( width/4, width/4, r );
    if( count >= HOS_T*HOS_STOP4 )
      hos_draw_label( width, width/2, count-HOS_T*HOS_STOP4 );
  }
}

const char authmsg1[] = "attach cd 01 /  ";
const char authmsg2[] = "enter author password  ";
const char authmsg3[] = "pass: ";

void hos_draw_authmsg_one(const char *authmsg, int n, int x, int y)
{
  char buf[BUFSIZ];

  strncpy( buf, authmsg, n );
  buf[n] = '\0';
  cairo_move_to( cairo, x, y );
  cairo_show_text( cairo, buf );
}

void hos_auth_draw(double width, double height, int count)
{
  static int n = 0;
  int n1, n2, n3, nl;

  n1 = strlen(authmsg1);
  n2 = strlen(authmsg2);
  n3 = strlen(authmsg3);
  cairo_identity_matrix( cairo );
  hos_draw_background( width, height );
  cairo_translate( cairo, width/2, height/2 );
  cairo_set_source_rgba( cairo, 1, 1, 1, 1 );
  cairo_set_font_size( cairo, height/28 );
  if( count == 0 ) n = 0;
  if( ( nl = ++n ) <= n1 ){
    hos_draw_authmsg_one( authmsg1, nl, -width/4, 0 );
  } else{
    if( ( nl -= n1 ) <= n2 ){
      hos_draw_authmsg_one( authmsg1, n1, -width/4, -height/16 );
      hos_draw_authmsg_one( authmsg2, nl, -width/4, 0 );
    } else{
      if( ( nl -= n2 ) <= n3 ){
        hos_draw_authmsg_one( authmsg1, n1, -width/4, -height/8 );
        hos_draw_authmsg_one( authmsg2, n2, -width/4, -height/16 );
        hos_draw_authmsg_one( authmsg3, nl, -width/4, 0 );
      } else{
        hos_draw_authmsg_one( authmsg1, n1, -width/4, -height/8 );
        hos_draw_authmsg_one( authmsg2, n2, -width/4, -height/16 );
        hos_draw_authmsg_one( authmsg3, n3, -width/4, 0 );
        if( count/HOS_BLINK_T % 2 == 0 ){
          cairo_text_extents_t te;
          cairo_text_extents( cairo, authmsg1, &te );
          cairo_rectangle( cairo, -width/4 + te.x_bearing + te.width/2, te.y_bearing, te.width/strlen(authmsg1), te.height );
          cairo_fill( cairo );
        }
      }
    }
  }
}

void *hos_anim(void *dummy)
{
  struct timespec ts = { 0, HOS_INTERVAL_NSEC * 1000 * 1000 };
  int count = 0;

  while( 1 ){
    if( !hos_pause ){
      if( count < HOS_T*HOS_STOP4 + HOS_LABEL_T )
        hos_draw( zxWindowWidth( &win ), zxWindowHeight( &win ), count++ );
      else
      if( count < HOS_T*HOS_STOP4 + HOS_LABEL_T + HOS_AUTH_T )
        hos_auth_draw( zxWindowWidth( &win ), zxWindowHeight( &win ), count++ - ( HOS_T*HOS_STOP4 + HOS_LABEL_T ) );
      else
        count = 0;
      zxFlush();
    }
    nanosleep( &ts, NULL );
  }
  return NULL;
}

#define WIDTH  640
#define HEIGHT 480

int main(int argc, char** argv)
{
  int quit_flag = 0;
  pthread_t thread;

  zxWindowCreateAndOpen( &win, 0, 0, WIDTH, HEIGHT );
  zxWindowKeyEnable( &win );
  hos_cairo_create();

  pthread_create( &thread, NULL, hos_anim, NULL );
  while( quit_flag != 1 ){
    switch( zxNextEvent() ){
    case Expose:
    case ConfigureNotify:
      if( zxevent.xexpose.count >= 1 ) break;
      hos_pause = true;
      hos_cairo_destroy();
      zxWindowResize( &win, zxevent.xexpose.width, zxevent.xexpose.height );
      hos_cairo_create();
      hos_pause = false;
      break;
    case KeyPress:
      switch( zxKeySymbol() ){
      case XK_q: case XK_Q: case XK_Escape: quit_flag = 1; break;
      }
      break;
    default: ;
    }
    if( zxWindowIsReceivedDeleteMsg( &win ) ) quit_flag = 1;
  }
  pthread_cancel( thread );
  hos_cairo_destroy();
  zxWindowDestroy( &win );
  return 0;
}
