#include "3rd_party/oxygine-framework/oxygine/text_utils/Aligner.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"

namespace oxygine
{
    namespace text
    {
        Aligner::Aligner(const TextStyle& Style, spSTDMaterial mt, const Font* font, float gscale, const Vector2& size)
            : m_style(Style),
              m_bounds(0, 0, 0, 0),
              m_width((int)size.x),
              m_height((int)size.y),
              m_mat(mt),
              m_font(font),
              m_scale(gscale),
              m_x(0),
              m_y(0),
              m_lineWidth(0)
        {
            m_line.reserve(50);
            m_lineSkip = (int)(m_font->getBaselineDistance() * m_style.baselineScale) + m_style.linesOffset;
            m_options = Style.options;
        }

        Aligner::~Aligner()
        {

        }

        qint32 Aligner::_alignX(qint32 rx)
        {
            qint32 tx = 0;
            switch (getStyle().hAlign)
            {
                case TextStyle::HALIGN_LEFT:
                case TextStyle::HALIGN_DEFAULT:
                    tx = 0;
                    break;
                case TextStyle::HALIGN_MIDDLE:
                    tx = m_width / 2 - rx / 2;
                    break;
                case TextStyle::HALIGN_RIGHT:
                    tx = m_width - rx;
                    break;
            }
            return tx;
        }

        qint32 Aligner::_alignY(qint32 ry)
        {
            qint32 ty = 0;

            switch (getStyle().vAlign)
            {
                case TextStyle::VALIGN_BASELINE:
                    ty = -getLineSkip();
                    break;
                case TextStyle::VALIGN_TOP:
                case TextStyle::VALIGN_DEFAULT:
                    ty = 0;
                    break;
                case TextStyle::VALIGN_MIDDLE:
                    ty = m_height / 2 - ry / 2;
                    break;
                case TextStyle::VALIGN_BOTTOM:
                    ty = m_height - ry;
                    break;
            }
            return ty;
        }

        void Aligner::begin()
        {
            m_x = 0;
            m_y = 0;

            m_width = int(m_width * m_scale);
            m_height = int(m_height * m_scale);

            m_bounds = Rect(_alignX(0), _alignY(0), 0, 0);
            nextLine();

        }

        void Aligner::end()
        {
            qint32 ry = m_y;

            if (getStyle().multiline)
            {
                nextLine();
                m_y -=  getLineSkip();
            }
            else
            {
                _alignLine(m_line);
            }

            m_bounds.setY(_alignY(ry));
            m_bounds.setHeight(ry);
        }

        qint32 Aligner::getLineWidth() const
        {
            return m_lineWidth;
        }

        qint32 Aligner::getLineSkip() const
        {
            return m_lineSkip;
        }

        void Aligner::_alignLine(line& ln)
        {
            if (!ln.empty())
            {
                //calculate real text width
                qint32 rx = 0;
                for (qint32 i = 0; i < ln.size(); ++i)
                {
                    Symbol& s = *ln[i];
                    rx = std::max(s.x + s.gl.advance_x, rx);
                }

                qint32 tx = _alignX(rx);

                for (qint32 i = 0; i < ln.size(); ++i)
                {
                    Symbol& s = *ln[i];
                    s.x += tx;
                }

                m_lineWidth = rx;

                m_bounds.setX(std::min(tx, m_bounds.getX()));
                m_bounds.setWidth(std::max(m_lineWidth, m_bounds.getWidth()));
            }
        }

        void Aligner::_nextLine(line& ln)
        {
            m_y += getLineSkip();
            _alignLine(ln);


            m_lineWidth = 0;

            m_x = 0;
        }

        void Aligner::nextLine()
        {
            _nextLine(m_line);
            m_line.clear();
        }

        float Aligner::getScale() const
        {
            return m_scale;
        }

        qint32 Aligner::putSymbol(Symbol& s)
        {
            if (m_line.empty() && s.code == ' ')
            {
                return 0;
            }
            m_line.push_back(&s);

            //optional remove?
            if (m_line.size() == 1 && s.gl.offset_x < 0)
            {
                m_x -= s.gl.offset_x;
            }

            s.x = m_x + s.gl.offset_x;
            s.y = m_y + s.gl.offset_y;
            m_x += s.gl.advance_x + getStyle().kerning;

            qint32 rx = s.x + s.gl.advance_x;
            m_lineWidth = std::max(rx, m_lineWidth);

            if (m_lineWidth > m_width && getStyle().multiline && (m_width > 0) && m_line.size() > 1)
            {
                qint32 lastWordPos = (int)m_line.size() - 1;
                for (; lastWordPos > 0; --lastWordPos)
                {
                    if (m_line[lastWordPos]->code == ' ' && m_line[lastWordPos - 1]->code != ' ')
                    {
                        break;
                    }
                }

                if (!lastWordPos)
                {
                    if (m_style.breakLongWords)
                    {
                        lastWordPos = (int)m_line.size() - 1;
                    }
                    else
                    {
                        return 0;
                    }
                }

                qint32 delta = (int)m_line.size() - lastWordPos;
                line leftPart;
                leftPart.resize(delta + 1);
                leftPart = line(m_line.begin() + lastWordPos, m_line.end());
                m_line.resize(lastWordPos);
                nextLine();
                for (qint32 i = 0; i < leftPart.size(); ++i)
                {
                    putSymbol(*leftPart[i]);
                }

                return 0;
            }
            assert(m_x > -1000);
            return 0;
        }
    }
}
