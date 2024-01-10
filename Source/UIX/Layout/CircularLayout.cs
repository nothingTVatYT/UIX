using FlaxEngine;
using FlaxEngine.GUI;

namespace UIX.Layout;

/// <summary>
/// A layout that arranges children around the center point with relative distance
/// </summary>
public class CircularLayout : BaseLayout
{
    public float RelativeDistance = 0.6f;
    public Float2 AngleMinMax = new(0, 360);
    public float AngleOffset;
    public bool Animate;
    public float AnimationSpeed = 8;

    private int _lastNumberChildren;

    /// <inheritdoc />
    public CircularLayout(ContainerControl container) : base(container)
    {
    }

    /// <inheritdoc />
    public override void PerformLayout(ContainerControl container, bool force = false)
    {
        if (Controls.Count == 0)
            return;
        var bounds = container.GetClientArea();
        var nChildren = Controls.Count;
        var angleStep = 360f;
        if (nChildren > 1)
        {
            if (AngleMinMax.X + 360 - AngleMinMax.Y < 5)
                angleStep = (AngleMinMax.Y - AngleMinMax.X) / nChildren;
            else
                angleStep = (AngleMinMax.Y - AngleMinMax.X) / (nChildren-1);
            if (nChildren > _lastNumberChildren)
            {
                for (var j = _lastNumberChildren; j < nChildren; j++)
                {
                    Controls[j].X = Controls[0].X;
                    Controls[j].Y = Controls[0].Y;
                }
            }
        }

        _lastNumberChildren = nChildren;
        var angleBegin = AngleMinMax.X + AngleOffset;
        var targetReached = true;
        for (var i = 0; i < nChildren; i++)
        {
            var child = Controls[i];
            var angle = angleBegin + i * angleStep;
            var posX = bounds.X + bounds.Width * (0.5f + Mathf.Cos(angle * Mathf.DegreesToRadians) / 2 * RelativeDistance) - child.Width / 2;
            var posY = bounds.Y + bounds.Height * (0.5f + Mathf.Sin(angle * Mathf.DegreesToRadians) / 2 * RelativeDistance) - child.Height / 2;
            
            if (Animate && i > 0)
            {
                if (Mathf.Abs(child.X - posX) > 5)
                    targetReached = false;
                child.X = Mathf.Lerp(child.X, posX, Time.DeltaTime * AnimationSpeed);
                child.Y = Mathf.Lerp(child.Y, posY, Time.DeltaTime * AnimationSpeed);
            }
            else
            {
                child.X = posX;
                child.Y = posY;
            }
        }

        IsLayoutDone = !Animate || targetReached;
    }
}